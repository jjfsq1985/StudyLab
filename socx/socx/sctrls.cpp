// sctrls.cpp : Csctrls ��ʵ��
#include "stdafx.h"
#include "sctrls.h"
#include <atlstr.h>

// Csctrls


STDMETHODIMP Csctrls::addparam(LONG lparam1, LONG lparam2)
{
    CString out;
    out.Format(L"out string %ld", lparam1 + lparam2);
    ::MessageBox(NULL, out, L"out", MB_OK | MB_SYSTEMMODAL);//�ÿؼ��޴��ھ��

    return S_OK;
}
