#include "stdafx.h"
#include "OraDbOperator.h"
#include <sstream>
using namespace std;


Connection OraDbOperator::m_con;

OraDbOperator::OraDbOperator()
{
}


OraDbOperator::~OraDbOperator()
{
}

bool OraDbOperator::OpenOraDb(const ostring& dbServerName, const ostring& cUser, const ostring& cPwd)
{
    Environment::Initialize(Environment::Default | Environment::Threaded);

    Environment::EnableWarnings(true); 

    m_con.Open(dbServerName, cUser, cPwd, Environment::SessionDefault);
    return true;
}

bool OraDbOperator::CloseOraDb()
{
    m_con.Close();
    Environment::Cleanup();
    return true;
}

int OraDbOperator::ExecuteCmd(const ostring& strSql)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        st.Execute(strSql);
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
            nRet = 0;
    }
    m_con.Commit();
    return nRet;
}

int OraDbOperator::ExecuteCmd(const ostring& strSql, Resultset& outResult)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        st.Execute(strSql);
        outResult = st.GetResultset();
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}

void OraDbOperator::BindData(Statement& st, DbParam& data)
{
    ostring strParam = data.Name;
        BindInfo::BindDirection  direction = BindInfo::Out;
        if (data.Direction == DbDirection::InOut)
            direction = BindInfo::InOut;
        else if (data.Direction == DbDirection::In)
            direction = BindInfo::Out;
        switch (data.ParType)
        {
        case DbDataType::TypeBoolean:            
            st.Bind(strParam, data.DbValue.bVal, direction);
            break;
        case DbDataType::TypeNumberInt:
            st.Bind(strParam, data.DbValue.nValue, direction);
            break;
        case DbDataType::TypeNumberInt64:
            st.Bind(strParam, (big_int)data.DbValue.int64Val, direction);
            break;
        case DbDataType::TypeFloat:
            st.Bind(strParam, data.DbValue.fltValue, direction);
            break;
        case DbDataType::TypeDouble:
            st.Bind(strParam, data.DbValue.dbValue, direction);
            break;
        case DbDataType::TypeString:
            st.Bind(strParam, data.ValStr, static_cast<unsigned int>(data.ValStr.size()), direction);
            break;
        case DbDataType::TypeDateTime:
            st.Bind(strParam, data.ValDate, direction);
            break;
        default:
            break;
        }
}

int OraDbOperator::ExecuteCmd(const ostring& strSql, DbParam* param, int nCount)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        st.Prepare(strSql);
        for (int i = 0; i < nCount; i++)
            BindData(st, param[i]);
        st.ExecutePrepared();
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}

int OraDbOperator::ExecuteCmd(const ostring& strSql, DbParam* param, int nCount, Resultset& outResult)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        st.Prepare(strSql);
        for (int i = 0; i < nCount; i++)
            BindData(st, param[i]);
        st.ExecutePrepared();
        outResult = st.GetResultset();
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}


int OraDbOperator::ExecuteProc(const ostring& cProcName)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        ostring strProc = L"begin " + cProcName + L"(); end;";
        st.Execute(strProc);
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}

int OraDbOperator::ExecuteProc(const ostring& cProcName, Resultset& outResult)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        ostring strProc = L"begin " + cProcName + L"(); end;";
        st.Execute(strProc);
        outResult = st.GetResultset();
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}

ostring OraDbOperator::GetParam(DbParam* param, int nCount)
{
    if (nCount < 1)
        return L"";
    ostring strRet = L"";
    for (int i = 0; i < nCount-1; i++)
    {
        strRet += param[i].Name + L",";
    }
    strRet += param[nCount-1].Name;
    return strRet;
}

int OraDbOperator::ExecuteProc(const ostring& cProcName, DbParam* param, int nCount)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        ostring strParamString = GetParam(param, nCount);
        ostring strProc = L"begin " + cProcName + L"(" + strParamString + L"); end;";
        st.Prepare(strProc);
        for (int i = 0; i < nCount; i++)
            BindData(st, param[i]);
        st.ExecutePrepared();
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}

int OraDbOperator::ExecuteProc(const ostring& cProcName, DbParam* param, int nCount, Resultset& outResult)
{
    int nRet = 1;
    try
    {
        Statement st(m_con);
        ostring strParamString = GetParam(param, nCount);
        ostring strProc = L"begin " + cProcName + L"(" + strParamString + L"); end;";
        st.Prepare(strProc);
        for (int i = 0; i < nCount; i++)
            BindData(st, param[i]);
        st.ExecutePrepared();
        outResult = st.GetResultset();
    }
    catch (Exception &ex)
    {
        wstringstream wss;
        wss << ex.what();
        Tprintf(wss.str().c_str());
        nRet = 0;
    }
    m_con.Commit();
    return nRet;
}


