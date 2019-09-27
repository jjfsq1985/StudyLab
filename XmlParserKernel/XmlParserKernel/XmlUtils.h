#pragma once
#include "xmlParser_private.h"


class XmlUtils
{
public:
	XmlUtils();
	~XmlUtils();


public:
	void AppendHead();
	xmlNode* GetRootNode();

	bool WriteXml(wchar_t* cPath);

	xmlNode* AllocNode(wchar_t* cName, wchar_t* cValue);

	void AppendNode(xmlNode* parent, xmlNode* node);


private:
	xml_document<wchar_t> m_doc;
};

