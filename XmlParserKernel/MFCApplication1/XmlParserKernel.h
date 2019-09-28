#pragma once

#include "xmldef.h"

XMLPARSER_BEGIN_DECLS

XML_PARSER_API bool InitUtil();
XML_PARSER_API xmlNode_t XmlRoot(char* cRoot);
XML_PARSER_API bool ReleaseUtil();

XML_PARSER_API bool WriteXml(char* cPath);

XML_PARSER_API xmlNode_t AllocNode(char* cName, char* cValue);
XML_PARSER_API void AppendNode(xmlNode_t parent, xmlNode_t node);

XML_PARSER_API xmlAttr_t AllocAttr(char* cAttrName, char* cValue);
XML_PARSER_API void AppendAttr(xmlNode_t node, xmlAttr_t attr);


XMLPARSER_END_DECLS