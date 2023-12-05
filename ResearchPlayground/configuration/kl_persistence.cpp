#include "predicates.h"
#include <vector>

#include "libxml/xmlwriter.h"
#include "libxml/parser.h"

#include "macro_config.h"
#include "function/function.h"
#include "common/util/utils.h"


constexpr auto CONFIGURATION_NODE = "CONFIGURATION";
constexpr auto CONFIG_NODE = "CONFIG";
constexpr auto LAYER_NODE = "LAYER";
constexpr auto ASSIGNMENT_NODE = "ASSIGNMENT";

static char buf[32] = "";
#define xmlNewPropStr(node, name, value); xmlNewProp((node), BAD_CAST(name), BAD_CAST(value));
#define xmlNewPropInt(node, name, value); sprintf(buf, "%d", value); xmlNewProp((node), BAD_CAST(name), BAD_CAST(buf));


static void AssignmentToXMLNode(const std::pair<const KEY_MapId_t, KLFunction>& pair, xmlNodePtr node)
{
    const KEY_MapId_t map_id = pair.first;
    const KLFunction& function = pair.second;

    KLFunction& function_default = FindDefaultFunctionByMapID(map_id);

    if (function.type == KL_FUNC_TYPE_KB) {
        xmlNewPropInt(node, "keymapid", map_id);
        xmlNewPropInt(node, "fid", function.id);
        xmlNewPropStr(node, "src", function_default.name);
        xmlNewPropStr(node, "dst", function.name);
        xmlNewPropInt(node, "type", function.type);
        xmlNewPropInt(node, "subtype", function.payload.kb.subType);
        xmlNewPropInt(node, "hid", function.payload.kb.hid);
    }
    else if (function.type == KL_FUNC_TYPE_MEDIA) {
        xmlNewPropInt(node, "keymapid", map_id);
        xmlNewPropInt(node, "fid", function.id);
        xmlNewPropStr(node, "src", function_default.name);
        xmlNewPropStr(node, "dst", function.name);
        xmlNewPropInt(node, "type", function.type);
        xmlNewPropInt(node, "hid", function.payload.media.hid);
    }
    else
    {
        xmlNewPropInt(node, "implement", 0);
    }
}

static void LayerToXMLNode(const std::unordered_map<KEY_MapId_t, KLFunction>& layer, xmlNodePtr layer_node)
{
    for (const auto& pair : layer)
    {
        xmlNodePtr assignment_node = xmlNewNode(NULL, BAD_CAST ASSIGNMENT_NODE);
        assert(assignment_node);

        KLFunction& function_default = FindDefaultFunctionByMapID(pair.first);
        if (pair.second.id == function_default.id)
            continue;

        AssignmentToXMLNode(pair, assignment_node);

        xmlAddChild(layer_node, assignment_node);
    }
}

void AssignmentConfigWrite(const char* filename)
{
    auto manager = KLFunctionConfigManager::GetInstance();

    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    assert(doc);

    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST CONFIGURATION_NODE);
    assert(root_node);

    xmlDocSetRootElement(doc, root_node);

    for (const auto& config : manager->m_ConfigList)
    {
        xmlNodePtr config_node = xmlNewNode(NULL, BAD_CAST CONFIG_NODE);
        assert(config_node);

        xmlNewPropStr(config_node, "name", config.name.c_str());
        {
            xmlNodePtr layer_node = xmlNewNode(NULL, BAD_CAST LAYER_NODE);
            assert(layer_node);
            xmlNewPropStr(layer_node, "type", "default");
            LayerToXMLNode(config.layers[KL_LAYER_TYPE_DEFAULT], layer_node);
            xmlAddChild(config_node, layer_node);
        }
        {
            xmlNodePtr layer_node = xmlNewNode(NULL, BAD_CAST LAYER_NODE);
            assert(layer_node);
            xmlNewPropStr(layer_node, "type", "fn1");
            LayerToXMLNode(config.layers[KL_LAYER_TYPE_FN1], layer_node);
            xmlAddChild(config_node, layer_node);
        }
        {
            xmlNodePtr layer_node = xmlNewNode(NULL, BAD_CAST LAYER_NODE);
            assert(layer_node);
            xmlNewPropStr(layer_node, "type", "fn2");
            LayerToXMLNode(config.layers[KL_LAYER_TYPE_FN2], layer_node);
            xmlAddChild(config_node, layer_node);
        }
        xmlAddChild(root_node, config_node);
    }

    xmlSaveFormatFile(filename, doc, 1);

    xmlFreeDoc(doc);
}

bool AssignmentConfigParseAssignment(xmlNodePtr node, std::pair<KEY_MapId_t, KLFunction>& pair)
{
    if (!xmlStrEqual(node->name, BAD_CAST ASSIGNMENT_NODE))
        return false;

    xmlChar* keymapid = xmlGetProp(node, BAD_CAST "keymapid");
    if (NULL == keymapid)
        return false;

    xmlChar* fid = xmlGetProp(node, BAD_CAST "fid");
    if (NULL == fid)
        return false;

    KEY_MapId_t mid = (KEY_MapId_t)atoi((char*)keymapid);
    if (mid <= KM_NONE || KM_MAXIMUM <= mid)
        return false;

    KLFunctionID functionId = (KLFunctionID)atoi((char*)fid);
    if (functionId <= KLF_NONE || KLF_MAXIMUM <= functionId)
        return false;

    pair.first = mid;
    pair.second = FindFunctionByFunctionID(functionId);

    return true;
}

bool AssignmentConfigParseLayer(xmlNodePtr node, std::unordered_map<KEY_MapId_t, KLFunction>& layer)
{
    if (!xmlStrEqual(node->name, BAD_CAST LAYER_NODE))
        return false;

    for (xmlNodePtr curNode = node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST ASSIGNMENT_NODE))
            continue;

        std::pair<KEY_MapId_t, KLFunction> pair;
        if (AssignmentConfigParseAssignment(curNode, pair))
            layer.insert(pair);
    }
    return true;
}

bool AssignmentConfigParseConfig(xmlNodePtr node, KLFunctionConfig& config)
{
    if (!xmlStrEqual(node->name, BAD_CAST CONFIG_NODE))
        return false;

    xmlChar* name = xmlGetProp(node, BAD_CAST "name");
    if (name) config.name = (char*)name;

    for (xmlNodePtr curNode = node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST LAYER_NODE))
            continue;

        xmlChar* layer_type = xmlGetProp(curNode, BAD_CAST "type");
        std::unordered_map<KEY_MapId_t, KLFunction> layer;
        if (xmlStrEqual(layer_type, BAD_CAST "default"))
        {
            if (AssignmentConfigParseLayer(curNode, layer))
                config.layers[KL_LAYER_TYPE_DEFAULT] = layer;
        }
        else if (xmlStrEqual(layer_type, BAD_CAST "fn1"))
        {
            if (AssignmentConfigParseLayer(curNode, layer))
                config.layers[KL_LAYER_TYPE_FN1] = layer;
        }
        else if (xmlStrEqual(layer_type, BAD_CAST "fn2"))
        {
            if (AssignmentConfigParseLayer(curNode, layer))
                config.layers[KL_LAYER_TYPE_FN2] = layer;
        }
        else
        {
            printf("error layer\n");
        }
    }
    return true;
}

void AssignmentConfigRead(const char* filename)
{
    xmlDocPtr doc = xmlParseFile(filename);
    if (!doc)
        return;

    xmlNodePtr root_node = xmlDocGetRootElement(doc);
    if (!root_node)
        return;
    assert(root_node->type == XML_ELEMENT_NODE);
    if (!xmlStrEqual(root_node->name, BAD_CAST CONFIGURATION_NODE))
        return;

    std::vector<KLFunctionConfig> configs;

    for (xmlNodePtr curNode = root_node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST CONFIG_NODE))
            continue;

        KLFunctionConfig config;
        if (AssignmentConfigParseConfig(curNode, config)) {
            configs.push_back(config);
        }
    }

    auto manager = KLFunctionConfigManager::GetInstance();
    manager->m_ConfigList = configs;

    xmlFreeDoc(doc);
}


constexpr auto MACROS_NODE = "MACROS";
constexpr auto MACRO_NODE = "MACRO";
constexpr auto ACTION_NODE = "ACTION";

void MacroConfigWrite(const char* filename)
{
    auto macro_manager = KLMacroConfigManager::GetInstance();

    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    assert(doc);

    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST MACROS_NODE);
    assert(root_node);
    
    xmlDocSetRootElement(doc, root_node);

    for (const auto& macro : macro_manager->m_ConfigList)
    {
        xmlNodePtr macro_node = xmlNewNode(NULL, BAD_CAST MACRO_NODE);
        assert(macro_node);

        xmlNewPropStr(macro_node, "name", macro.name.c_str());
        xmlNewPropInt(macro_node, "id", macro.id);

        for (const auto& action : macro.pair_actions)
        {
            xmlNodePtr action_node = xmlNewNode(NULL, BAD_CAST ACTION_NODE);
            assert(action_node);

            KLFunction& function = FindFunctionByFunctionID(action.mFunctionID);

            xmlNewPropStr(action_node, "name", function.name);
            xmlNewPropInt(action_node, "fid", action.mFunctionID);

            if (function.type == KL_FUNC_TYPE_KB)
            {
                xmlNewPropInt(action_node, "hid", function.payload.kb.hid);
            }

            xmlNewPropInt(action_node, "start", action.mFrameStart);
            xmlNewPropInt(action_node, "end", action.mFrameEnd);

            xmlAddChild(macro_node, action_node);
        }
        xmlAddChild(root_node, macro_node);
    }

    xmlSaveFormatFile(filename, doc, 1);

    xmlFreeDoc(doc);
}

bool MacroConfigParseActionPair(xmlNodePtr action_node, KLMacroActionPairInfo& action_pair)
{
    xmlChar* fid = xmlGetProp(action_node, BAD_CAST "fid");
    if (fid) action_pair.mFunctionID = (KLFunctionID)atoi((char*)fid);

    xmlChar* start = xmlGetProp(action_node, BAD_CAST "start");
    if (start) action_pair.mFrameStart = atoi((char*)start);

    xmlChar* end = xmlGetProp(action_node, BAD_CAST "end");
    if (end) action_pair.mFrameEnd = atoi((char*)end);

    return true;
}

bool MacroConfigParseMacro(xmlNodePtr macro_node, KLMacro& macro)
{
    if (!xmlStrEqual(macro_node->name, BAD_CAST MACRO_NODE))
        return false;

    xmlChar* name = xmlGetProp(macro_node, BAD_CAST "name");
    if (name) macro.name = (char*)name;

    xmlChar* id = xmlGetProp(macro_node, BAD_CAST "id");
    if (id) macro.id = atoi((char*)id);

    for (xmlNodePtr curNode = macro_node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST ACTION_NODE))
            continue;

        KLMacroActionPairInfo action_pair;
        if (MacroConfigParseActionPair(curNode, action_pair)) {
            macro.pair_actions.push_back(action_pair);
        }
    }
    return true;
}

void MacroConfigRead(const char* filename)
{
    xmlDocPtr doc = xmlParseFile(filename);
    if (!doc)
        return;

    xmlNodePtr root_node = xmlDocGetRootElement(doc);
    if (!root_node)
        return;
    assert(root_node->type == XML_ELEMENT_NODE);
    if (!xmlStrEqual(root_node->name, BAD_CAST MACROS_NODE))
        return;

    std::vector<KLMacro> configs;

    for (xmlNodePtr curNode = root_node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST MACRO_NODE))
            continue;

        KLMacro macro;
        if (MacroConfigParseMacro(curNode, macro)) {
            configs.push_back(macro);
        }
    }

    auto manager = KLMacroConfigManager::GetInstance();
    manager->m_ConfigList = configs;

    xmlFreeDoc(doc);
}
