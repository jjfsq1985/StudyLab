#include "stdafx.h"
#include "XmlUtils.h"
#include <iostream>
#include <fstream>
using namespace std;


XmlUtils::XmlUtils()
{
}


XmlUtils::~XmlUtils()
{
}


void XmlUtils::AppendHead()
{
	xml_node<wchar_t>* rot = m_doc.allocate_node(rapidxml::node_pi, m_doc.allocate_string(L"xml version=\"1.0\" encoding=\"utf-8\""));
	m_doc.append_node(rot);
	xml_node<wchar_t>* node = m_doc.allocate_node(rapidxml::node_element, L"ArrayOfPrimParBase", L"data");
	xml_attribute<wchar_t>* attr1 = m_doc.allocate_attribute(L"xmlns:xsd", L"http://www.w3.org/2001/XMLSchema");
	xml_attribute<wchar_t>* attr2 = m_doc.allocate_attribute(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");
	node->append_attribute(attr1);
	node->append_attribute(attr2);
	m_doc.append_node(node);
}

xmlNode* XmlUtils::GetRootNode()
{
	return (xmlNode*)m_doc.first_node(L"ArrayOfPrimParBase");
}

bool XmlUtils::WriteXml(wchar_t* cPath)
{
	if (wcslen(cPath) < 8)
		return false;

	wofstream ofs(cPath);
	locale china("zh_CN.UTF-8");
	ofs.imbue(china);
	ofs << m_doc;
	return true;
}


void XmlUtils::AppendNode(xmlNode* parent, xmlNode* node)
{
	if (parent == NULL || node == NULL)
		return;
	parent->append_node(node);
}

xmlNode* XmlUtils::AllocNode(wchar_t* cName, wchar_t* cValue)
{
	if (cName == NULL)
		return NULL;
	if (cValue == NULL || wcslen(cValue) == 0)
		return (xmlNode*)m_doc.allocate_node(rapidxml::node_element, cName, NULL);
	else
		return (xmlNode*)m_doc.allocate_node(rapidxml::node_element, cName, cValue);
}