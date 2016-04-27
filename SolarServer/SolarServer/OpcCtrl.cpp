#include "stdafx.h"
#include "OpcCtrl.h"

#include "OPCAuto_i.c"

CComModule _Module;//必须，否则事件接受器响应报错

OpcCtrl::OpcCtrl()
    :m_ActiveOPCGroup(NULL)
{
    CoInitializeEx(NULL,COINIT_MULTITHREADED);
    CoCreateInstance(CLSID_OPCServer, NULL, CLSCTX_SERVER, IID_IUnknown, (void **)&m_pUnk);
    m_nCount = 0;
    m_DataChangeFunc = NULL;
    m_pDataChangeCtrl = NULL;
}


OpcCtrl::~OpcCtrl()
{
    if (m_ActiveOPCGroup != NULL)
    {
        DispEventUnadvise(m_ActiveOPCGroup);//取消订阅事件
    }
    m_pUnk->Release();
    CoUninitialize();
}

bool OpcCtrl::getListOfServers(_bstr_t sNode, vector<_bstr_t>& serverlist)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    CComVariant varNode(sNode.GetBSTR());
    CComVariant serverVar;
    hr = iServer->GetOPCServers(varNode, &serverVar);
    if (FAILED(hr))
        return false;
    if (serverVar.vt == (VT_BSTR | VT_ARRAY) )
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(serverVar.parray, 1, &uBound);
        ::SafeArrayGetLBound(serverVar.parray, 1, &lBound);//OPC lBound从1开始
        for (LONG i = lBound; i <= uBound; i++)
        {
            OLECHAR *cSvrName = NULL;
            SafeArrayGetElement(serverVar.parray, &i, (void*)&cSvrName);
            _bstr_t sSvrName(cSvrName);
            serverlist.push_back(sSvrName);
        }
        return true;
    }
    else
    {
        return false;
    }    
}

bool OpcCtrl::connectServer(_bstr_t serverName, _bstr_t sNode)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    
    hr = iServer->Connect(serverName, CComVariant(sNode.GetBSTR()));
    if (FAILED(hr))
        return false;
    return true;
}

bool OpcCtrl::DisconnectServer()
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    hr = iServer->Disconnect();
    if (FAILED(hr))
        return false;
    return true;
}

bool OpcCtrl::BrowserBranches(vector<OpcItem>& vecBranches)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    ATL::CComPtr<OPCBrowser> iBrowser;
    hr = iServer->CreateBrowser(&iBrowser);
    if (FAILED(hr))
        return false;

    iBrowser->MoveToRoot();
    iBrowser->ShowBranches();

    LONG lCount = 0;
    iBrowser->get_Count(&lCount);
    for (LONG ii = 1; ii <= lCount; ii++)
    {
        OLECHAR *branchName = NULL;
        iBrowser->Item(CComVariant(ii), &branchName);
        OLECHAR *branchID = NULL;
        iBrowser->GetItemID(branchName, &branchID);
        OpcItem branch;
        branch.sItem = branchName;
        branch.sItemId = branchID;
        branch.sParent = "";
        branch.bLeafs = true;
        vecBranches.push_back(branch);
    }

    //以下可省略
    iBrowser->ShowLeafs(CComVariant(false));
    LONG lRootCnt = 0;
    iBrowser->get_Count(&lRootCnt);
    for (LONG ll = 1; ll <= lRootCnt; ll++)
    {
        OLECHAR *branchName = NULL;
        iBrowser->Item(CComVariant(ll), &branchName);
        OLECHAR *branchID = NULL;
        iBrowser->GetItemID(branchName, &branchID);
        OpcItem branch;
        branch.sItem = branchName;
        branch.sItemId = branchID;
        branch.sParent = "";
        branch.bLeafs = false;
        vecBranches.push_back(branch);
    }
    /////////////////////////以上可省略/////////////////////////////////////////////////

    return  true;
}

bool OpcCtrl::BrowserLeafs(_bstr_t sBranch, vector<OpcItem>& vecLeafs)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    ATL::CComPtr<OPCBrowser> iBrowser;
    hr = iServer->CreateBrowser(&iBrowser);
    if (FAILED(hr))
        return false;

    SAFEARRAYBOUND cBounds;
    cBounds.lLbound = 1;
    cBounds.cElements = 1;
    SAFEARRAY* pArray = SafeArrayCreate(VT_BSTR, 1, &cBounds);
    DWORD ii = 1;
    SafeArrayPutElement(pArray, (LONG*)&ii, sBranch.GetBSTR());
    hr = iBrowser->MoveTo(&pArray);
    SafeArrayDestroy(pArray);

    iBrowser->ShowBranches();

    /////////////////////////以下可省略//////////////////////////////////////////////
    LONG lCount = 0;
    iBrowser->get_Count(&lCount);
    for (LONG ii = 1; ii <= lCount; ii++)
    {
    OLECHAR *branchName = NULL;
    iBrowser->Item(CComVariant(ii), &branchName);
    OLECHAR *branchId = NULL;
    iBrowser->GetItemID(branchName, &branchId);
    OpcItem branch;
    branch.sItem = branchName;
    branch.sItemId = branchId;
    branch.sParent = "";
    branch.bLeafs = true;
    vecLeafs.push_back(branch);
    }
    //以上可省略

    iBrowser->ShowLeafs(CComVariant(false));
    LONG lRootCnt = 0;
    iBrowser->get_Count(&lRootCnt);
    for (LONG ll = 1; ll <= lRootCnt; ll++)
    {
        OLECHAR *LeafName = NULL;
        iBrowser->Item(CComVariant(ll), &LeafName);
        OLECHAR *LeafID = NULL;
        iBrowser->GetItemID(LeafName, &LeafID);
        OpcItem branch;
        branch.sItem = LeafName;
        branch.sItemId = LeafID;
        branch.sParent = sBranch;
        branch.bLeafs = false;
        vecLeafs.push_back(branch);
    }

    return  true;
}

bool OpcCtrl::QueryItemProperties(_bstr_t item, LONG& PropCount, vector<LONG>& vecPropID, vector<_bstr_t>& vecPropDesc, vector<LONG>& vecDataType)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    LONG Count = 0;
    SAFEARRAY *PropID = NULL;
    SAFEARRAY *PropDesc = NULL;
    SAFEARRAY *DataType = NULL;
    iServer->QueryAvailableProperties(item, &Count, &PropID, &PropDesc, &DataType);
    PropCount = Count;
    for (LONG i = 1; i <= Count; i++)
    {
        LONG propId = 0;
        SafeArrayGetElement(PropID, (LONG*)&i, (void *)&propId);
        vecPropID.push_back(propId);
        OLECHAR *cDesc = NULL;
        SafeArrayGetElement(PropDesc, (LONG*)&i, (void *)&cDesc);
        _bstr_t propDescribe(cDesc);
        vecPropDesc.push_back(propDescribe);
        LONG propDataType = 0;
        SafeArrayGetElement(DataType, (LONG*)&i, (void *)&propDataType);
        vecDataType.push_back(propDataType);
    }
    if(PropID != NULL)
        SafeArrayDestroy(PropID);
    if (PropDesc != NULL)
        SafeArrayDestroy(PropDesc);
    if (DataType != NULL)
        SafeArrayDestroy(DataType);

    return true;
}

bool OpcCtrl::GetItemProperties(_bstr_t item, LONG PropCount, vector<LONG> vecPropID, vector<VARIANT>& vecPropValue, vector<LONG>& vecErrors)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    SAFEARRAYBOUND saBound;
    saBound.lLbound = 1;
    saBound.cElements = PropCount;
    SAFEARRAY  *pPropID = SafeArrayCreate(VT_I4, 1, &saBound);
    for (LONG i = 1; i <= PropCount; i++)
    {
        SafeArrayPutElement(pPropID, (LONG*)&i, (void *)&vecPropID[i-1]);
    }
    SAFEARRAY  *pPropValue = NULL;
    SAFEARRAY  *pPropErr = NULL;
    iServer->GetItemProperties(item, PropCount, &pPropID, &pPropValue, &pPropErr);

    for (LONG i = 1; i <= PropCount; i++)
    {
        VARIANT varValue;
        SafeArrayGetElement(pPropValue, (LONG*)&i, (void *)&varValue);
        vecPropValue.push_back(varValue);
        LONG lErr = 0;
        SafeArrayGetElement(pPropErr, (LONG*)&i, (void *)&lErr);
        vecErrors.push_back(lErr);
    }
    SafeArrayDestroy(pPropID);

    if (pPropValue != NULL)
        SafeArrayDestroy(pPropValue);
    if (pPropErr != NULL)
        SafeArrayDestroy(pPropErr);

    return true;
}

LONG OpcCtrl::NextHandle()
{
    m_nCount = (m_nCount % 10) + m_nCount + 1;
    return m_nCount;
}

bool OpcCtrl::AddGroup(_bstr_t groupName, LONG UpdateRate, bool bSubscribed)
{
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    ATL::CComPtr<IOPCGroups> iGroups;
    iServer->get_OPCGroups((OPCGroups**)&iGroups);
    iGroups->put_DefaultGroupIsActive(VARIANT_TRUE);
    iGroups->put_DefaultGroupUpdateRate(UpdateRate);
    iGroups->Add(CComVariant(groupName.GetBSTR()), (OPCGroup**)&m_ActiveOPCGroup);
    m_ActiveOPCGroup->put_IsActive(VARIANT_TRUE);
    VARIANT_BOOL  varSubscribed = (bSubscribed) ? VARIANT_TRUE : VARIANT_FALSE;
    m_ActiveOPCGroup->put_IsSubscribed(varSubscribed);
    m_ActiveOPCGroup->put_ClientHandle(NextHandle());
    DispEventAdvise(m_ActiveOPCGroup);//订阅事件响应
    return true;
}

bool OpcCtrl::RemoveGroup(_bstr_t groupName)
{
    if (m_ActiveOPCGroup == NULL)
        return false;
    ATL::CComPtr<IOPCAutoServer> iServer;
    HRESULT hr = m_pUnk->QueryInterface(IID_IOPCAutoServer, (void**)&iServer);
    if (FAILED(hr))
        return false;
    DispEventUnadvise(m_ActiveOPCGroup);//取消订阅事件
    m_ActiveOPCGroup->put_IsSubscribed(VARIANT_FALSE);
    m_ActiveOPCGroup->put_IsActive(VARIANT_FALSE);
    ATL::CComPtr<IOPCGroups> iGroups;
    iServer->get_OPCGroups((OPCGroups**)&iGroups);
    LONG serverHandle = 0;
    m_ActiveOPCGroup->get_ServerHandle(&serverHandle);
    iGroups->Remove(CComVariant(serverHandle));
    return true;
}

bool OpcCtrl::AddOpcItems(vector<_bstr_t> vecItemsId, vector<LONG>& vecServerHandle, vector<LONG>& vecClientHandle, vector<LONG>& vecResult)
{
    if (m_ActiveOPCGroup == NULL || vecItemsId.size() < 2)
        return false;
    vecServerHandle.clear();
    ATL::CComPtr<OPCItems> iOpcItems;
    HRESULT hr = m_ActiveOPCGroup->get_OPCItems((OPCItems**)&iOpcItems);
    if (FAILED(hr))
        return false;
    int nAddCount = vecItemsId.size();
    SAFEARRAYBOUND saBound;
    saBound.lLbound = 1;
    saBound.cElements = nAddCount;
    SAFEARRAY  *pItemsID = SafeArrayCreate(VT_BSTR, 1, &saBound);
    SAFEARRAY *pClientHandle = SafeArrayCreate(VT_I4, 1, &saBound);
    for (LONG i = 1; i <= nAddCount; i++)
    {
        BSTR cItemId = vecItemsId[i - 1].GetBSTR();
        SafeArrayPutElement(pItemsID, (LONG*)&i, (void *)cItemId);
        LONG client = NextHandle();
        SafeArrayPutElement(pClientHandle, (LONG*)&i, (void *)&client);
    }
    SAFEARRAY  *pServerHandles = NULL;
    SAFEARRAY  *pErrs = NULL;
    iOpcItems->AddItems(nAddCount, &pItemsID, &pClientHandle, &pServerHandles, &pErrs, CComVariant(), CComVariant());

    //记录pServerHandle
    for (LONG i = 1; i <= nAddCount; i++)
    {
        LONG serverHandle;
        SafeArrayGetElement(pServerHandles, (LONG*)&i, (void *)&serverHandle);
        vecServerHandle.push_back(serverHandle);
        LONG clientHandle;
        SafeArrayGetElement(pClientHandle, (LONG*)&i, (void *)&clientHandle);
        vecClientHandle.push_back(clientHandle);
        LONG result;
        SafeArrayGetElement(pErrs, (LONG*)&i, (void *)&result);
        vecResult.push_back(result);
    }

    SafeArrayDestroy(pItemsID);
    SafeArrayDestroy(pClientHandle);

    if (pServerHandles != NULL)
        SafeArrayDestroy(pServerHandles);
    if (pErrs != NULL)
        SafeArrayDestroy(pErrs);

    return true;
}

bool OpcCtrl::AddOpcItem(_bstr_t ItemID, LONG& lSvrHandle, LONG& lClientHandle, LONG& lResult)
{
    if (m_ActiveOPCGroup == NULL)
        return false;
    ATL::CComPtr<OPCItems> iOpcItems;
    HRESULT hr = m_ActiveOPCGroup->get_OPCItems((OPCItems**)&iOpcItems);
    if (FAILED(hr))
        return false;
    ATL::CComPtr<OPCItem> iItemOpc;
    lClientHandle = NextHandle();
    lResult = iOpcItems->AddItem(ItemID.GetBSTR(), lClientHandle, (OPCItem**)&iItemOpc);
    if (SUCCEEDED(lResult))
        iItemOpc->get_ServerHandle(&lSvrHandle);
    return true;
}

bool OpcCtrl::RemoveItems(vector<LONG> vecItemsHandle)
{
    if (m_ActiveOPCGroup == NULL)
        return false;
    ATL::CComPtr<OPCItems> iOpcItems;
    HRESULT hr = m_ActiveOPCGroup->get_OPCItems((OPCItems**)&iOpcItems);
    if (FAILED(hr))
        return false;
    int nRemoveCount = vecItemsHandle.size();
    SAFEARRAYBOUND saBound;
    saBound.lLbound = 1;
    saBound.cElements = nRemoveCount;
    SAFEARRAY *pHandle = SafeArrayCreate(VT_I4, 1, &saBound);
    for (LONG i = 1; i <= nRemoveCount; i++)
    {
        LONG lHandle = vecItemsHandle[i - 1];
        SafeArrayPutElement(pHandle, (LONG*)&i, (void *)&lHandle);
    }
    SAFEARRAY  *pErrs = NULL;
    iOpcItems->Remove(nRemoveCount, &pHandle, &pErrs);
    
    SafeArrayDestroy(pHandle);

    if (pErrs != NULL)
        SafeArrayDestroy(pErrs);
    return true;
}

void OpcCtrl::SetOpcDataChange(void *pSrcCtrl, CallbackDataChange pFunc)
{
    m_pDataChangeCtrl = pSrcCtrl;
    m_DataChangeFunc = pFunc;
}

STDMETHODIMP OpcCtrl::OnDataChange(long TransactionID, long NumItems, SAFEARRAY  *ClientHandles, SAFEARRAY  *ItemValues, SAFEARRAY  *Qualities, SAFEARRAY  *TimeStamps)
{
    if (m_DataChangeFunc == NULL)
        return S_FALSE;
    vector<long> vecClientHandle;
    vector<VARIANT> vecData;
    vector<long> vecQuality;
    vector<SYSTEMTIME> vecStamp;
    for (LONG i = 1; i <= NumItems; i++)
    {
        LONG  clientHdl = 0;
        SafeArrayGetElement(ClientHandles, &i, (void*)&clientHdl);
        vecClientHandle.push_back(clientHdl);
        VARIANT itemVal;
        SafeArrayGetElement(ItemValues, &i, (void*)&itemVal);
        vecData.push_back(itemVal);
        LONG quality = 0;
        SafeArrayGetElement(Qualities, &i, (void*)&quality);
        vecQuality.push_back(quality);
        DATE timstamp = 0;
        SafeArrayGetElement(TimeStamps, &i, (void*)&timstamp);
        SYSTEMTIME systime;
        VariantTimeToSystemTime(timstamp, &systime);
        vecStamp.push_back(systime);
    }
    m_DataChangeFunc(m_pDataChangeCtrl, NumItems, vecClientHandle, vecData, vecQuality, vecStamp);
    return S_OK;
}

QString OpcCtrl::GetDataType(LONG dataType)
{
    //等价于VARENUM
    switch ((VARENUM)dataType)
    {
    case VARENUM::VT_I1:                         return "char";
    case VARENUM::VT_UI1:                        return  "unsigned char";
    case VARENUM::VT_I2:                         return "short";
    case VARENUM::VT_UI2:                        return "unsigned short";
    case VARENUM::VT_I4:                         return "int32";
    case VARENUM::VT_UI4:                        return "unsigned int32";
    case VARENUM::VT_R4:                         return "float";
    case VARENUM::VT_R8:                         return "double";
    case VARENUM::VT_CY:                         return  "decimal";
    case VARENUM::VT_BOOL:                       return "bool";
    case VARENUM::VT_DATE:                       return "DateTime";
    case VARENUM::VT_BSTR:                       return "string";
    case VARENUM::VT_ARRAY | VARENUM::VT_I1:      return "sbyte[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_UI1:     return "byte[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_I2:      return "short[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_UI2:     return "ushort[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_I4:      return "int[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_UI4:     return "uint[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_R4:      return "float[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_R8:      return "double[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_CY:      return "decimal[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BOOL:    return "bool[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_DATE:    return "DateTime[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BSTR:    return "string[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_VARIANT: return "object[]";
    default:   return "object";
    }
}

QString OpcCtrl::GetDataValue(VARIANT varValue)
{
    switch (varValue.vt)
    {
    case VARENUM::VT_I1:                         return QString("%1").arg(varValue.cVal);
    case VARENUM::VT_UI1:                        return   QString("%1").arg(varValue.bVal);
    case VARENUM::VT_I2:                         return QString("%1").arg(varValue.iVal);
    case VARENUM::VT_UI2:                        return QString("%1").arg(varValue.uiVal);
    case VARENUM::VT_I4:                         return QString("%1").arg(varValue.intVal);
    case VARENUM::VT_UI4:                        return QString("%1").arg(varValue.uintVal);
    case VARENUM::VT_R4:                         return QString("%1").arg(varValue.fltVal);
    case VARENUM::VT_R8:                         return QString("%1").arg(varValue.dblVal);
    case VARENUM::VT_CY:                         return  QString("%1").arg((qlonglong)varValue.cyVal.int64);
    case VARENUM::VT_BOOL:                       return QString("%1").arg(varValue.boolVal);
    case VARENUM::VT_DATE:
    {
        SYSTEMTIME systime;
        VariantTimeToSystemTime(varValue.date, &systime);
        return GetTimeStamp(systime);
        QDate dateval(systime.wYear, systime.wMonth, systime.wDay);
        QTime timeval(systime.wHour, systime.wMinute, systime.wSecond);
        QDateTime timeVal(dateval, timeval);
        return timeVal.toString("yyyy-MM-dd HH:mm:ss");
    }
    case VARENUM::VT_BSTR:
    {
        wstring strVal = varValue.bstrVal;
        return QString::fromStdWString(strVal);
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_I1:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        CHAR *cVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&cVal);
            strAllVal += QString("%1;").arg(*cVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&cVal);
        strAllVal += QString("%1").arg(*cVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_UI1:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        BYTE *bVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&bVal);
            strAllVal += QString("%1;").arg(*bVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&bVal);
        strAllVal += QString("%1").arg(*bVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_I2:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        short *shortVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&shortVal);
            strAllVal += QString("%1;").arg(*shortVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&shortVal);
        strAllVal += QString("%1").arg(*shortVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_UI2:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        ushort *ushortVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&ushortVal);
            strAllVal += QString("%1;").arg(*ushortVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&ushortVal);
        strAllVal += QString("%1").arg(*ushortVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_I4:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        int *intVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&intVal);
            strAllVal += QString("%1;").arg(*intVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&intVal);
        strAllVal += QString("%1").arg(*intVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_UI4:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        unsigned int *uintVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&uintVal);
            strAllVal += QString("%1;").arg(*uintVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&uintVal);
        strAllVal += QString("%1").arg(*uintVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_R4:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        float *fltVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&fltVal);
            strAllVal += QString("%1;").arg(*fltVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&fltVal);
        strAllVal += QString("%1").arg(*fltVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_R8:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        double *dblVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&dblVal);
            strAllVal += QString("%1;").arg(*dblVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&dblVal);
        strAllVal += QString("%1").arg(*dblVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_CY: return "decimal[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BOOL: return "VARIANT_BOOL[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_DATE: return "DATE[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BSTR: return "BSTR[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_VARIANT: return "Variant[]";
    default:   return "Empty";
    }
}

QString OpcCtrl::GetTimeStamp(SYSTEMTIME sysTime)
{
    QDate dateval(sysTime.wYear, sysTime.wMonth, sysTime.wDay);
    QTime timeval(sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    QDateTime timeVal(dateval, timeval);
    return timeVal.toString("yyyy-MM-dd HH:mm:ss");
}