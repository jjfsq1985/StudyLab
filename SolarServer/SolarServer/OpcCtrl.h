#pragma once
#include <atlbase.h>  //CComPtr
#include <atlcom.h>
#include <comdef.h>//_bstr_t
#include "OPCAuto.h"
#include <vector>
using namespace std;

typedef struct tagOpcItem
{
    _bstr_t sItemId;
    _bstr_t sItem;
    _bstr_t sParent;
    bool bLeafs;
}OpcItem;

typedef void (CALLBACK  *CallbackDataChange)(void *pSrcCtrl, long NumItems, const  vector<long>& vecClientHandle, const vector<VARIANT>& vecData, const vector<long>& vecQuality, const vector<SYSTEMTIME>& vecStamp);

class OpcCtrl : public IDispEventImpl<1, OpcCtrl, &DIID_DIOPCGroupEvent, &LIBID_OPCAutomation,1,0>
{
public:
    OpcCtrl();
    ~OpcCtrl();
    BEGIN_SINK_MAP(OpcCtrl)
        SINK_ENTRY_EX(1, DIID_DIOPCGroupEvent, 0x1/**\DataChange*/, OnDataChange)
    END_SINK_MAP()

public:
    bool getListOfServers(_bstr_t sNode, vector<_bstr_t>& serverlist);
    bool connectServer(_bstr_t serverName, _bstr_t sNode = "");
    bool DisconnectServer();
    bool BrowserBranches(vector<OpcItem>& vecBranches);
    bool BrowserLeafs(_bstr_t sBranch, vector<OpcItem>& vecLeafs);
    bool QueryItemProperties(_bstr_t item, LONG& PropCount, vector<LONG>& vecPropID, vector<_bstr_t>& vecPropDesc, vector<LONG>& vecDataType);
    bool GetItemProperties(_bstr_t item, LONG PropCount, vector<LONG> vecPropID, vector<VARIANT>& vecPropValue, vector<INT>& vecErrors);

    void SetOpcDataChange(void *pSrcCtrl, CallbackDataChange pFunc);

public:
    bool AddGroup(_bstr_t groupName, LONG UpdateRate, bool bSubscribed);
    bool RemoveGroup(_bstr_t groupName);
    bool AddOpcItems(vector<_bstr_t> vecItemsId, vector<LONG>& vecServerHandle, vector<LONG>& vecClientHandle, vector<LONG>& vecAccessRight, vector<INT>& vecResult);
    bool AddOpcItem(_bstr_t ItemID, LONG& lSvrHandle, LONG& lClientHandle, LONG& lAccessRight, INT& lResult);
    bool RemoveItems(vector<LONG> vecItemsId);
    bool SyncRead(LONG nItems, const vector<LONG>& vecSvrHander, vector<VARIANT>& vecValue, vector<INT>& vecErrors, vector<LONG>& vecQualities, vector<SYSTEMTIME>& vecTimeStamps);
    bool SyncWrite(LONG nItems, const vector<LONG>& vecSvrHander, const vector<VARIANT>& vecValue, vector<INT>& vecErrors);

protected:
        STDMETHOD(OnDataChange)(long TransactionID, long NumItems, SAFEARRAY  *ClientHandles, SAFEARRAY  *ItemValues, SAFEARRAY  *Qualities, SAFEARRAY  *TimeStamps);

private:
    CallbackDataChange m_DataChangeFunc;
    void *m_pDataChangeCtrl;
    LONG NextHandle();
private:
    LONG m_nCount;   //ClientHander累计器
    IUnknown *m_pUnk;
    int m_nActiveIndex;   //当前活动的Group索引
    vector<ATL::CComPtr<IOPCGroup> > m_vecActiveOPCGroup;
};

