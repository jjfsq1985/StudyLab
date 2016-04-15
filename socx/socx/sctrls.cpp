// sctrls.cpp : Csctrls 的实现
#include "stdafx.h"
#include "sctrls.h"
#include <atlstr.h>

// Csctrls


STDMETHODIMP Csctrls::addparam(LONG lparam1, LONG lparam2)
{
    CString out;
    out.Format(L"out string %ld", lparam1 + lparam2);
    ::MessageBox(NULL, out, L"out", MB_OK | MB_SYSTEMMODAL);//该控件无窗口句柄

    return S_OK;
}
