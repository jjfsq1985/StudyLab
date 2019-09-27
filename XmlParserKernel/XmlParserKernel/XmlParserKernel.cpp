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

xmlNode_t GetRoot()
{
	if (g_pXml == NULL)
		return NULL;
	return g_pXml->GetRootNode();
}

bool ReleaseUtil()
{
	if (g_pXml != NULL)
		delete g_pXml;
	g_pXml = NULL;

	return true;
}

bool WriteXml(wchar_t* cPath)
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

xmlNode_t AllocNode(wchar_t* cName, wchar_t* cValue)
{
	if (g_pXml == NULL)
		return NULL;
	return g_pXml->AllocNode(cName, cValue);
}