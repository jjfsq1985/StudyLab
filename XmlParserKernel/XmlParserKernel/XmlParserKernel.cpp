// XmlParserKernel.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "XmlParserKernel.h"
#include "XmlUtils.h"

XmlUtils *g_pXml = NULL;

bool InitUtil()
{
	if (g_pXml != NULL)
		delete g_pXml;
	g_pXml = NULL;
	g_pXml = new XmlUtils();
	g_pXml->AppendHead();
	return true;
}

xmlNode_t WriteXmlRoot(char* cRoot)
{
	if (g_pXml == NULL)
		return NULL;
	return g_pXml->WriteRootNode(cRoot);
}

bool ReleaseUtil()
{
	if (g_pXml != NULL)
		delete g_pXml;
	g_pXml = NULL;

	return true;
}

bool WriteXml(char* cPath)
{
	if (g_pXml == NULL || cPath == NULL)
		return false;
	return g_pXml->WriteXml(cPath);
}

void AppendNode(xmlNode_t parent, xmlNode_t node)
{
	if (g_pXml == NULL)
		return;
	g_pXml->AppendNode(parent, node);
}

xmlNode_t AllocNode(char* cName, char* cValue)
{
	if (g_pXml == NULL)
		return NULL;
	return g_pXml->AllocNode(cName, cValue);
}

xmlAttr_t AllocAttr(char* cAttrName, char* cValue)
{
	if (g_pXml == NULL)
		return NULL;
	return g_pXml->AllocAttrib(cAttrName, cValue);
}

void AppendAttr(xmlNode_t node, xmlAttr_t attr)
{
	if (g_pXml == NULL)
		return;
	g_pXml->AppendAttrib(node, attr);
}

xmlNode_t ReadXml(char* cPath)
{
	if (g_pXml == NULL || cPath == NULL)
		return NULL;
	return g_pXml->ReadXml(cPath);
}

bool ReadXmlRoot(xmlNode_t rootNode, char* cRoot)
{
	if (g_pXml == NULL || rootNode == NULL || cRoot == NULL)
		return false;
	return g_pXml->ReadRootNode(rootNode,cRoot);
}

