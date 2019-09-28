#pragma once
#include "xmlParser_private.h"
#include <string>
using namespace std;

class XmlUtils
{
public:
	XmlUtils();
	~XmlUtils();


public:
	void AppendHead();
	xmlNode* WriteRootNode(char *cRoot);

	bool WriteXml(char* cPath);

	xmlNode* AllocNode(char* cName, char* cValue);

	void AppendNode(xmlNode* parent, xmlNode* node);

	xmlAttr* AllocAttrib(char* cAttrName, char* cValue);

	void AppendAttrib(xmlNode* node, xmlAttr* attr);

public:
	xmlNode* ReadXml(char* cPath);
	bool ReadRootNode(xmlNode* rootNode, char* sName);

private:
	rapidxml::file<char>* m_xmlfile;
	xml_document<char> m_doc;
};

