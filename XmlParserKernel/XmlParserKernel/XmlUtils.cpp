#include "stdafx.h"
#include <stdlib.h>
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
	xml_node<char>* rot = m_doc.allocate_node(rapidxml::node_pi, m_doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
	m_doc.append_node(rot);
}

xmlNode* XmlUtils::RootNode(char* cRoot)
{
	if (cRoot == NULL || strlen(cRoot) == 0)
		return NULL;
	xml_node<char>* node = m_doc.allocate_node(rapidxml::node_element, cRoot, "data");
	m_doc.append_node(node);
	return (xmlNode*)node;
}

bool XmlUtils::WriteXml(char* cPath)
{
	if (cPath == NULL || strlen(cPath) < 8)
		return false;

	ofstream ofs(cPath);
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

xmlNode* XmlUtils::AllocNode(char* cName, char* cValue)
{
	if (cName == NULL)
		return NULL;
	if (cValue == NULL || strlen(cValue) == 0)
		return (xmlNode*)m_doc.allocate_node(rapidxml::node_element, cName, NULL);
	else
		return (xmlNode*)m_doc.allocate_node(rapidxml::node_element, cName, cValue);
}

xmlAttr* XmlUtils::AllocAttrib(char* cAttrName, char* cValue)
{
	if (cAttrName == NULL || cValue == NULL)
		return NULL;
	return (xmlAttr*)m_doc.allocate_attribute(cAttrName, cValue);
}

void XmlUtils::AppendAttrib(xmlNode* node, xmlAttr* attr)
{
	if (node == NULL || attr == NULL)
		return;
	node->append_attribute(attr);
}
