// DES Tool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#include "resource.h"		// ������


// CDESToolApp:
// �йش����ʵ�֣������ DES Tool.cpp
//

class CDESToolApp : public CWinApp
{
public:
	CDESToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDESToolApp theApp;