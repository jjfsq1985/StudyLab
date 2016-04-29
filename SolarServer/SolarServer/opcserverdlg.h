#ifndef OPCSERVERDLG_H
#define OPCSERVERDLG_H

#include <QDialog>
#include "ui_opcserverdlg.h"

#include "dlgaddgroup.h"
#include "dlgadditems.h"
#include <map>
using namespace  std;

class OPCServerDlg : public QDialog
{
    Q_OBJECT

public:
    OPCServerDlg(QWidget *parent = 0);
    ~OPCServerDlg();

public:
    void setOpcCtrl(class OpcCtrl *pCtrl);
    void InitOpcServer(const wstring& strSvrName, const  vector<GroupParam>& vecGroups, const map<wstring, vector<ItemInfo> >& mapItems);
    void GetOpcData(wstring& strSvrName, vector<GroupParam>& vecGroups, map<wstring, vector<ItemInfo> >& mapItems);
    void EventResponse_DataChange(long NumItems, const  vector<long>& vecClientHandle, const vector<VARIANT>& vecData, const vector<long>& vecQuality, const vector<SYSTEMTIME>& vecStamp);

private:
    int IndexFromClientHandler(LONG nClientHandle, const vector<ItemInfo>& vecItem);
    int IndexFromServerHandler(LONG nServerHandle, const vector<ItemInfo>& vecItem);
    QString GetAccessRight(LONG lRight);

private:
    static void CALLBACK ResponseDataChange(void *pSrcCtrl, long NumItems, const  vector<long>& vecClientHandle, const vector<VARIANT>& vecData, const vector<long>& vecQuality, const vector<SYSTEMTIME>& vecStamp);

private:
    virtual void contextMenuEvent(QContextMenuEvent *e);

private slots:
    void on_OpcGrouptree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_opcitemList_itemChanged(QTableWidgetItem *item);
    void on_btnEdit_clicked();
    void on_btnconnect_clicked();
    void on_btnDisconnect_clicked();
    void AddGroup();
    void BrowserItems();
    void DelGroup();
    void AddItems();
    void RemoveItem();

private:
    typedef struct _tagWriteInfo
    {
        LONG ItemSvrHandler;
        LONG nDataType;
        bool bDirty;
        VARIANT varValue;
    }WriteInfo;
    wstring m_strServerName;
    wstring m_strSelectedGroup;
    map<wstring, vector<ItemInfo> > m_mapItems;
    vector<WriteInfo> m_vecSelectedItems;
     vector<GroupParam> m_vecGroups;
    class OpcCtrl *m_pOpcCtrl;
private:
    bool m_bEdit;
    QMenu *m_pContextMenu;
    QAction *m_pAddGroup;
    QAction *m_pBrowserItems;
    QAction *m_pDelGroup;
    QAction *m_pAddItems;
    QAction *m_pRemoveItem;
    Ui::DlgOpcServer ui;
};

#endif // OPCSERVERDLG_H
