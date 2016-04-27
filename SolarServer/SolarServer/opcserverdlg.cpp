#include "stdafx.h"
#include "opcserverdlg.h"
#include "opclistdlg.h"
#include "OpcCtrl.h"
#include "browseitemsdlg.h"

OPCServerDlg::OPCServerDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_pOpcCtrl = NULL;
    ui.OpcGrouptree->header()->hide();
    m_mapItems.clear();


    m_pContextMenu = new QMenu(this);
    m_pAddGroup = new QAction(tr("Add Group..."),this);
    m_pBrowserItems = new QAction(tr("Browser Items..."), this);
    m_pDelGroup = new QAction(tr("Remove Group"), this);
    m_pAddItems = new QAction(tr("Add Items..."), this);
    m_pRemoveItem = new QAction(tr("Remove Item"), this);

    connect(m_pAddGroup, &QAction::triggered, this, &OPCServerDlg::AddGroup);
    connect(m_pBrowserItems, &QAction::triggered, this, &OPCServerDlg::BrowserItems);
    connect(m_pDelGroup, &QAction::triggered, this, &OPCServerDlg::DelGroup);
    connect(m_pAddItems, &QAction::triggered, this, &OPCServerDlg::AddItems);
    connect(m_pRemoveItem, &QAction::triggered, this, &OPCServerDlg::RemoveItem);
    
}

OPCServerDlg::~OPCServerDlg()
{

}

void OPCServerDlg::setOpcCtrl(class OpcCtrl *pCtrl)
{
    m_pOpcCtrl = pCtrl;
}

void OPCServerDlg::on_btnconnect_clicked()
{
    OPCListDlg lstDlg(this);
    lstDlg.setOpcCtrl(m_pOpcCtrl);
    lstDlg.enumServer();
    if (lstDlg.exec() == Accepted)
    {
        wstring strName = lstDlg.GetServerName();
        bool bRet = m_pOpcCtrl->connectServer(strName.c_str(), "localhost");
        if (bRet)
        {
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui.OpcGrouptree);
            rootItem->setText(0, QString::fromStdWString(strName));
            ui.OpcGrouptree->addTopLevelItem(rootItem);
        }
    }
}

void OPCServerDlg::on_btnDisconnect_clicked()
{
    m_pOpcCtrl->DisconnectServer();
}

void OPCServerDlg::contextMenuEvent(QContextMenuEvent *e)
{
    QTreeWidgetItem *curitem = ui.OpcGrouptree->currentItem();
    if (curitem == NULL)
        return;
    if (curitem->parent() == NULL)
    {
        //server
        m_pContextMenu->clear();
        m_pContextMenu->addAction(m_pAddGroup);
        m_pContextMenu->addAction(m_pBrowserItems);
    }
    else if (curitem->parent() != ui.OpcGrouptree->topLevelItem(0))
    {
        //item
        m_pContextMenu->clear();
        m_pContextMenu->addAction(m_pRemoveItem);
    }
    else
    {
        //group
        m_pContextMenu->clear();
        m_pContextMenu->addAction(m_pAddItems);
    }
    m_pContextMenu->exec(QCursor::pos());
    e->accept();
}

void OPCServerDlg::AddGroup()
{
    DlgAddGroup addgropdlg(this);
    addgropdlg.SetAddedGroup(m_vecGroups);
    if (addgropdlg.exec() == Accepted)
    {
        wstring strGroup = L"Default";
        bool bActived = true;
        int nUpdateRate = 1000;
        addgropdlg.GetGroupParam(strGroup,bActived,nUpdateRate);
        GroupParam par;
        par.strGroupName = strGroup;
        par.bActive = bActived;
        par.nUpdateRate = nUpdateRate;
        m_vecGroups.push_back(par);
        m_pOpcCtrl->AddGroup(strGroup.c_str(), nUpdateRate, bActived);
        QTreeWidgetItem *pTopItem = ui.OpcGrouptree->topLevelItem(0);
        QTreeWidgetItem *child = new QTreeWidgetItem(pTopItem);
        child->setText(0, QString::fromStdWString(strGroup));
        QTreeWidgetItem *curitem = ui.OpcGrouptree->currentItem();
        Q_ASSERT(curitem == pTopItem);
        pTopItem->addChild(child);
    }
}

void OPCServerDlg::BrowserItems()
{
    BrowseItemsDlg browseDlg(this);
    browseDlg.setOpcCtrl(m_pOpcCtrl);
    browseDlg.browseItems();
    browseDlg.exec();
}

void OPCServerDlg::DelGroup()
{
    QTreeWidgetItem *pTopItem = ui.OpcGrouptree->topLevelItem(0);
    QTreeWidgetItem *curitem = ui.OpcGrouptree->currentItem();
    pTopItem->removeChild(curitem);
    wstring strName = curitem->text(0).toStdWString();
    m_pOpcCtrl->RemoveGroup(strName.c_str());
}

void OPCServerDlg::AddItems()
{
    dlgAddItems addItemsdlg(this);
    addItemsdlg.setOpcCtrl(m_pOpcCtrl);
    addItemsdlg.browseItems();
    if (addItemsdlg.exec() == Accepted)
    {
        vector<ItemInfo> vecAddItems;
        addItemsdlg.GetSelectedItems(vecAddItems);
        QTreeWidgetItem *curitem = ui.OpcGrouptree->currentItem();
        wstring strGroup = curitem->text(0).toStdWString();
        Q_ASSERT(curitem->parent() == ui.OpcGrouptree->topLevelItem(0));

        for (vector<ItemInfo>::iterator it = vecAddItems.begin(); it != vecAddItems.end(); it++)
        {
            QTreeWidgetItem *child = new QTreeWidgetItem(curitem);
            child->setText(0, QString::fromStdWString((*it).strOpcItemId));
            child->setData(0, Qt::UserRole, QVariant((*it).OpcItemSvrHandle));
            curitem->addChild(child);
        }
        vector<ItemInfo>& vecSelectedItems = m_mapItems[strGroup];
        vecSelectedItems.assign(vecAddItems.begin(), vecAddItems.end());

        ui.opcitemList->setRowCount(vecSelectedItems.size());
        int nRow = 0;
        for (vector<ItemInfo>::iterator it = vecSelectedItems.begin(); it != vecSelectedItems.end(); it++)
        {
            wstring strId = (*it).strOpcItemId;
            QTableWidgetItem *pTableItem = new QTableWidgetItem(QString::fromStdWString(strId));
            ui.opcitemList->setItem(nRow, 0, pTableItem);
            nRow++;
        }

        m_pOpcCtrl->SetOpcDataChange(this,&OPCServerDlg::ResponseDataChange);

    }
}

void CALLBACK OPCServerDlg::ResponseDataChange(void *pSrcCtrl, long NumItems, const  vector<long>& vecClientHandle, const vector<VARIANT>& vecData, const vector<long>& vecQuality, const vector<SYSTEMTIME>& vecStamp)
{
    if (pSrcCtrl == NULL)
        return;
    OPCServerDlg *pDlg = (OPCServerDlg *)pSrcCtrl;
    pDlg->EventResponse_DataChange(NumItems, vecClientHandle, vecData, vecQuality, vecStamp);
}

void OPCServerDlg::RemoveItem()
{
    QTreeWidgetItem *curitem = ui.OpcGrouptree->currentItem();
    QTreeWidgetItem *GroupItem = curitem->parent();
    wstring strItemId = curitem->text(0).toStdWString();
    wstring strGroup = GroupItem->text(0).toStdWString();
    LONG serverHandle = curitem->data(0, Qt::UserRole).toInt();
    GroupItem->removeChild(curitem);
    vector<LONG> vecRemove;
    vecRemove.push_back(serverHandle);
    m_pOpcCtrl->RemoveItems(vecRemove);

    vector<ItemInfo>& vecSelectedItems = m_mapItems[strGroup];
    for (vector<ItemInfo>::iterator it = vecSelectedItems.begin(); it != vecSelectedItems.end(); it++)
    {
        if ((*it).OpcItemSvrHandle == serverHandle)
        {
            vecSelectedItems.erase(it);
            break;
        }
    }
}

int OPCServerDlg::GetItemIndexInVector(LONG nClientHandle, const vector<ItemInfo>& vecItem)
{
    int nRow = -1;
    int nIndex = 0;
    for (vector<ItemInfo>::const_iterator it = vecItem.begin(); it != vecItem.end(); it++)
    {
        if ((*it).OpcItemClientHandle == nClientHandle)
        {
            nRow = nIndex;
            break;
        }
        nIndex++;
    }
    return nRow;

}

void OPCServerDlg::EventResponse_DataChange(long NumItems, const  vector<long>& vecClientHandle, const vector<VARIANT>& vecData, const vector<long>& vecQuality, const vector<SYSTEMTIME>& vecStamp)
{
    if (NumItems <= 0)
        return;
    QTableWidgetItem *pTableItem = NULL;
    for (map<wstring, vector<ItemInfo> >::iterator itfind = m_mapItems.begin(); itfind != m_mapItems.end(); itfind++)
    {
        vector<ItemInfo>& vecSelectedItems = itfind->second;
        for (int i = 0; i < NumItems; i++)
        {
            int nRow = GetItemIndexInVector(vecClientHandle[i], vecSelectedItems);
            if (nRow >= 0)
            {
                QString strValue = m_pOpcCtrl->GetDataValue(vecData[nRow]);
                pTableItem = new QTableWidgetItem(strValue);
                ui.opcitemList->setItem(nRow, 1, pTableItem);
                pTableItem = new QTableWidgetItem(QString("%1").arg(vecQuality[nRow]));
                ui.opcitemList->setItem(nRow, 2, pTableItem);
                QString strTime = m_pOpcCtrl->GetTimeStamp(vecStamp[nRow]);
                pTableItem = new QTableWidgetItem(strTime);
                ui.opcitemList->setItem(nRow, 3, pTableItem);
            }
        }
    }

}