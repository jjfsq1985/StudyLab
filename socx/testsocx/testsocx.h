
// testsocx.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestsocxApp: 
// �йش����ʵ�֣������ testsocx.cpp
//

class CtestsocxApp : public CWinApp
{
public:
	CtestsocxApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestsocxApp theApp;