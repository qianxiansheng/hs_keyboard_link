#include "predicates.h"
#include <vector>

#include "libxml/xmlwriter.h"
#include "libxml/parser.h"

#include "macro_config.h"
#include "function/function.h"
#include "common/util/utils.h"

constexpr auto ROOT_NODE = "MACROS";
constexpr auto MACRO_NODE = "MACRO";
constexpr auto ACTION_NODE = "ACTION";

constexpr auto FILE_NAME = "macros.xml";

void MacroConfigWrite()
{
    char buf[32] = "";
#define xmlNewPropStr(node, name, value); xmlNewProp((node), BAD_CAST(name), BAD_CAST(value));
#define xmlNewPropInt(node, name, value); sprintf(buf, "%d", value); xmlNewProp((node), BAD_CAST(name), BAD_CAST(buf));

    auto macro_manager = KLMacroConfigManager::GetInstance();

    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    assert(doc);

    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST ROOT_NODE);
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

    xmlSaveFormatFile(FILE_NAME, doc, 1);

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

void MacroConfigRead()
{
    xmlDocPtr doc = xmlParseFile(FILE_NAME);
    if (!doc)
        return;

    xmlNodePtr root_node = xmlDocGetRootElement(doc);
    if (!root_node)
        return;
    assert(root_node->type == XML_ELEMENT_NODE);
    if (!xmlStrEqual(root_node->name, BAD_CAST ROOT_NODE))
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
}
