#pragma once

#include "xmldef.h"

XMLPARSER_BEGIN_DECLS

XML_PARSER_API bool InitUtil();
XML_PARSER_API bool ReleaseUtil();

XML_PARSER_API bool WriteXml(char* cPath);
XML_PARSER_API xmlNode_t WriteXmlRoot(char* cRoot);
XML_PARSER_API xmlNode_t AllocNode(char* cName, char* cValue);
XML_PARSER_API void AppendNode(xmlNode_t parent, xmlNode_t node);
XML_PARSER_API xmlAttr_t AllocAttr(char* cAttrName, char* cValue);
XML_PARSER_API void AppendAttr(xmlNode_t node, xmlAttr_t attr);

XML_PARSER_API xmlNode_t ReadXml(char* cPath);
XML_PARSER_API bool ReadXmlRoot(xmlNode_t rootNode, char* cRoot);
//XML_PARSER_API void ParseNode(xmlNode_t node, int& nAttrCount, int& nChildNodeCount);
//XML_PARSER_API xmlNode_t ParseChildSiblingNode(xmlNode_t node, int nChildIndex);
//XML_PARSER_API bool ParseAttr(xmlNode_t node, xmlAttr_t attr);
//XML_PARSER_API bool ReadNode(xmlNode_t node, char* sName, char* cValue);
//XML_PARSER_API bool ReadAttr(xmlAttr_t attr, char* cAttr, char* cValue);


XMLPARSER_END_DECLS