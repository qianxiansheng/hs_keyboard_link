#include "language.h"

#include <iostream>
#include <assert.h>

#include "util/utils.h"

#include "libxml/parser.h"

KLLanguageManager* KLLanguageManager::s_Instance = nullptr;
std::once_flag KLLanguageManager::s_OnceFlag;

static constexpr auto PROFILE_NODE = "PROFILE";
static constexpr auto LANGUAGE_NODE = "LANGUAGE";
static constexpr auto ENTRY_NODE = "ENTRY";

bool LanguageConfigParseKeyPair(xmlNodePtr action_node, std::pair<KLLangTextKey, std::string>& pair)
{
    xmlChar* type = xmlGetProp(action_node, BAD_CAST "type");
    if (!type) return false;

    xmlChar* value = xmlGetProp(action_node, BAD_CAST "value");
    if (!value) return false;

    pair.first = (KLLangTextKey)atoi((char*)type);
    pair.second = (char*)value;

    return true;
}


bool LanguageConfigParseLanguage(xmlNodePtr node, std::unordered_map<KLLangTextKey, std::string>& language)
{
    if (!xmlStrEqual(node->name, BAD_CAST LANGUAGE_NODE))
        return false;

    for (xmlNodePtr curNode = node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST ENTRY_NODE))
            continue;

        std::pair<KLLangTextKey, std::string> pair;
        if (LanguageConfigParseKeyPair(curNode, pair)) {
            language.insert(pair);
        }
    }
    return true;
}


void KLLanguageManager::LoadConfig(const char* filename)
{
    auto a = utils::getFileAbsolutePath(filename);
    xmlDocPtr doc = xmlParseFile(a.c_str());
    if (!doc)
        return;

    xmlNodePtr root_node = xmlDocGetRootElement(doc);
    if (!root_node)
        return;
    assert(root_node->type == XML_ELEMENT_NODE);
    if (!xmlStrEqual(root_node->name, BAD_CAST PROFILE_NODE))
        return;

    std::unordered_map<KLLangType, std::unordered_map<KLLangTextKey, std::string>> map;

    for (xmlNodePtr curNode = root_node->children; curNode; curNode = curNode->next) {
        if (!xmlStrEqual(curNode->name, BAD_CAST LANGUAGE_NODE))
            continue;

        xmlChar* type = xmlGetProp(curNode, BAD_CAST "type");
        if (!type)
            continue;

        KLLangType ltype = (KLLangType)atoi((char*)type);

        std::pair<KLLangType, std::unordered_map<KLLangTextKey, std::string>> language;
        language.first = ltype;
        if (LanguageConfigParseLanguage(curNode, language.second)) {
            map.insert(language);
        }
    }

    auto manager = KLLanguageManager::GetInstance();
    manager->map = map;

    xmlFreeDoc(doc);
}

