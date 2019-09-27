#pragma once

#include "xmldef.h"

XMLPARSER_BEGIN_DECLS

XML_PARSER_API bool InitUtil();
XML_PARSER_API xmlNode_t GetRoot();
XML_PARSER_API bool ReleaseUtil();

XML_PARSER_API bool WriteXml(wchar_t* cPath);

XML_PARSER_API xmlNode_t AllocNode(wchar_t* cName, wchar_t* cValue);
XML_PARSER_API void AppendNode(xmlNode_t parent, xmlNode_t node);

XMLPARSER_END_DECLS