#include "predicates.h"

#include "libxml/xmlwriter.h"

#include "macro_config.h"

constexpr auto ROOT_NODE = "MACROS";
constexpr auto MACRO_NODE = "MACROS";

void MacroConfigWrite()
{
    auto macro_manager = KLMacroConfigManager::GetInstance();

    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    assert(doc);

    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST ROOT_NODE);
    assert(root_node);
    
    xmlNodePtr current_node = xmlDocSetRootElement(doc, root_node);
    assert(current_node);

    for (uint32_t i = 0; i < macro_manager->m_ConfigList.size(); ++i)
    {
        xmlNodePtr macro_node = xmlNewNode(NULL, BAD_CAST ROOT_NODE);
        assert(macro_node);

    }

    xmlFreeDoc(doc);
}

void MacroConfigRead()
{

}
