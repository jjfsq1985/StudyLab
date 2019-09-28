#ifndef XML_DEF_H
#define XML_DEF_H

#ifdef XMLPARSERKERNEL_EXPORTS
#define XML_PARSER_API __declspec(dllexport)
#else
#define XML_PARSER_API __declspec(dllimport)
#endif

#ifdef  __cplusplus
#define XMLPARSER_BEGIN_DECLS  extern "C" {
#define XMLPARSER_END_DECLS    }
#else
#define XMLPARSER_BEGIN_DECLS
#define XMLPARSER_END_DECLS
#endif


XMLPARSER_BEGIN_DECLS

const int MAX_XML_STR_LEN = 1024;

typedef class xmlNode* xmlNode_t;

typedef class xmlAttr* xmlAttr_t;

XMLPARSER_END_DECLS

#endif//
