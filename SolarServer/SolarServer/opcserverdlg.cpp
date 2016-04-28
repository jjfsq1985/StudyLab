#include "stdafx.h"
#include "opcserverdlg.h"
#include "opclistdlg.h"
#include "OpcCtrl.h"
#include "browseitemsdlg.h"
#include "CommonTranslate.h"

OPCServerDlg::OPCServerDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_pOpcCtrl = NULL;
    ui.OpcGrouptree->header()->hide();
    //ui.opcitemList->setEditTriggers(QTableWidget::NoEditTriggers);//整个qtablewidget都不可编辑
    m_mapItems.clear();
    m_bEdit = false;

    m_pContextMenu = new QMenu(this);
    m_pAddGroup = new QAction(tr("Add Group..."), this);
    m_pBrowserItems = new QAction(tr("Browser Items..."), this);
    m_pDelGroup = new QAction(tr("Remove Group"), this);
    m_pAddItems = new QAction(tr("Add Items..."), this);
    m_pRemoveItem = new QAction(tr("Remove Item"), this);

    connect(m_pAddGroup, &QAction::triggered, this, &OPCServerDlg::AddGroup);
    connect(m_pBrowserItems, &QAction::triggered, this, &OPCServerDlg::BrowserItems);
    connect(m_pDelGroup, &QAction::triggered, this, &OPCServerDlg::DelGroup);
    connect(m_pAddItems, &QAction::triggered, this, &OPCServerDlg::AddItems);
    connect(m_pRemoveItem, &QAction::triggered, this, &OPCServerDlg::RemoveItem);

    m_strServerName = L"";
    m_strSelectedGroup = L"";
    ui.opcitemList->horizontalHeader()->resizeSection(3, 150);
    ui.btnEdit->setEnabled(false);
}

OPCServerDlg::~OPCServerDlg()
{
    m_pOpcCtrl->SetOpcDataChange(NULL, NULL);
    m_vecGroups.clear();
    m_mapItems.clear();
}

void OPCServerDlg::InitOpcServer(const wstring& strSvrName, const  vector<GroupParam>& vecGroups, const map<wstring, vector<ItemInfo> >& mapItems)
{
    m_strServerName = strSvrName;
    m_vecGroups = vecGroups;
    m_mapItems = mapItems;
    QTreeWidgetItem *rootItem = NULL;
    if (m_strServerName != L"")
    {
        rootItem = new QTreeWidgetItem(ui.OpcGrouptree);
        rootItem->setText(0, QString::fromStdWString(m_strServerName));
        ui.OpcGrouptree->addTopLevelItem(rootItem);
    }

    if (m_vecGroups.size() > 0 && rootItem != NULL)
    {
        m_strSelectedGroup = m_vecGroups[0].strGroupName;
        for (vector<GroupParam>::iterator it = m_vecGroups.begin(); it != m_vecGroups.end(); it++)
        {
            wstring strGroup = (*it).strGroupName;
            QTreeWidgetItem *pGroup = new QTreeWidgetItem(rootItem);
            pGroup->setText(0, QString::fromStdWString(strGroup));
            rootItem->addChild(pGroup);
            
            if (m_mapItems.size() > 0)
            {
                map<wstring, vector<ItemInfo> >::iterator itfind = m_mapItems.find(strGroup);
                if (itfind != m_mapItems.end())
                {
                    vector<ItemInfo>& vecItems = itfind->second;
                    for (vector<ItemInfo>::iterator it = vecItems.begin(); it != vecItems.end(); it++)
                    {
                        QTreeWidgetItem *child = new QTreeWidgetItem(pGroup);
                        child->setText(0, QString::fromStdWString((*it).strOpcItemId));
                        child->setData(0, Qt::UserRole, QVariant((*it).OpcItemSvrHandle));//Serverhander
                        pGroup->addChild(child);
                    }
                    if (m_strSelectedGroup == strGroup && !pGroup->isExpanded())
                        ui.OpcGrouptree->expandItem(pGroup);
                }
            }
        }
        if (!rootItem->isExpanded())
            ui.OpcGrouptree->expandItem(rootItem);
    }
    if (m_mapItems.size() > 0)
    {
        map<wstring, vector<ItemInfo> >::iterator itfind = m_mapItems.find(m_strSelectedGroup);
        if (itfind != m_mapItems.end())
        {
            vector<ItemInfo>& vecItems = itfind->second;
            ui.opcitemList->setRowCount(vecItems.size());
            ui.btnEdit->setEnabled(true);
            vector<LONG> vecSvrHandle;
            int nRow = 0;
            QTableWidgetItem *pTableItem = NULL;
            for (vector<ItemInfo>::iterator it = vecItems.begin(); it != vecItems.end(); it++)
            {
                wstring strId = (*it).strOpcItemId;
                pTableItem = new QTableWidgetItem(QString::fromStdWString(strId));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(nRow, 0, pTableItem);
                vecSvrHandle.push_back((*it).OpcItemSvrHandle);
                nRow++;
            }
            vector<VARIANT> vecValue;
            vector<LONG> vecErr;
            vector<LONG> vecQuality;
            vector<SYSTEMTIME> vecStamp;
            m_pOpcCtrl->SyncRead(nRow, vecSvrHandle, vecValue, vecErr, vecQuality, vecStamp);
            for (int i = 0; i < nRow; i++)
            {
                pTableItem = new QTableWidgetItem(CommonTranslate::GetDataValue(vecValue[i]));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(i, 1, pTableItem);
                if (vecQuality.size() > i)
                {
                    pTableItem = new QTableWidgetItem(QString("%1").arg(vecQuality[i]));
                    pTableItem->setFlags(Qt::ItemIsEnabled);
                    ui.opcitemList->setItem(i, 2, pTableItem);
                }
                if (vecStamp.size() > i)
                {
                    pTableItem = new QTableWidgetItem(CommonTranslate::GetTimeStamp(vecStamp[i]));
                    pTableItem->setFlags(Qt::ItemIsEnabled);
                    ui.opcitemList->setItem(i, 3, pTableItem);
                }
                pTableItem = new QTableWidgetItem(QString("0x%1").arg(vecErr[i], 2, 16, QChar('0')));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(i, 4, pTableItem);
            }
        }
    }

    m_pOpcCtrl->SetOpcDataChange(this, &OPCServerDlg::ResponseDataChange);
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
            m_strServerName = strName;
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui.OpcGrouptree);
            rootItem->setText(0, QString::fromStdWString(strName));
            ui.OpcGrouptree->addTopLevelItem(rootItem);
        }
    }
}

void OPCServerDlg::on_btnDisconnect_clicked()
{
    m_pOpcCtrl->DisconnectServer();
    m_strServerName = L"";
    m_vecGroups.clear();
    m_mapItems.clear();
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
        m_pContextMenu->addAction(m_pDelGroup);
        m_pContextMenu->addAction(m_pAddItems);
    }
    m_pContextMenu->exec(QCursor::pos());
    e->accept();
}

void OPCServerDlg::AddGroup()
{
    DlgAddGroup addgropdlg(this);
    addgropdlg.SetAddedGroup(m_vecGroups);
    if (addgropdlg.exec() == QDialog::Accepted)
    {
        wstring strGroup = L"Default";
        bool bActived = true;
        int nUpdateRate = 1000;
        addgropdlg.GetGroupParam(strGroup, bActived, nUpdateRate);
        GroupParam par;
        par.strGroupName = strGroup;
        par.bActive = bActived;
        par.nUpdateRate = nUpdateRate;
        m_vecGroups.push_back(par);
        m_pOpcCtrl->AddGroup(strGroup.c_str(), nUpdateRate, bActived);
        m_strSelectedGroup = strGroup;//记录当前活动的Groups
        QTreeWidgetItem *pTopItem = ui.OpcGrouptree->topLevelItem(0);
        QTreeWidgetItem *child = new QTreeWidgetItem(pTopItem);
        child->setText(0, QString::fromStdWString(strGroup));
        QTreeWidgetItem *curitem = ui.OpcGrouptree->currentItem();
        Q_ASSERT(curitem == pTopItem);
        pTopItem->addChild(child);
        if (!pTopItem->isExpanded())
            ui.OpcGrouptree->expandItem(pTopItem);
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
    if (m_pOpcCtrl->RemoveGroup(strName.c_str()))
    {
        for (vector<GroupParam>::iterator it = m_vecGroups.begin(); it != m_vecGroups.end(); it++)
        {
            if ((*it).strGroupName == strName)
            {
                m_vecGroups.erase(it);
                break;
            }
        }
        QTreeWidgetItem *pCurGroup = pTopItem->child(0);
        if (pCurGroup != NULL)
        {
            m_strSelectedGroup = pCurGroup->text(0).toStdWString();
            ui.OpcGrouptree->setCurrentItem(pCurGroup, 0);
        }
    }
}

void OPCServerDlg::AddItems()
{
    dlgAddItems addItemsdlg(this);
    addItemsdlg.setOpcCtrl(m_pOpcCtrl);
    addItemsdlg.browseItems();
    if (addItemsdlg.exec() == QDialog::Accepted)
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
            child->setData(0, Qt::UserRole, QVariant((*it).OpcItemSvrHandle));//Serverhander
            curitem->addChild(child);
        }
        vector<ItemInfo>& vecSelectedItems = m_mapItems[strGroup];
        vecSelectedItems.insert(vecSelectedItems.end(),vecAddItems.begin(), vecAddItems.end());
        if (!curitem->isExpanded())
            ui.OpcGrouptree->expandItem(curitem);

        ui.opcitemList->setRowCount(vecSelectedItems.size());
        ui.btnEdit->setEnabled(true);
        int nRow = 0;
        QTableWidgetItem *pTableItem = NULL;
        vector<LONG> vecServerHandle;
        for (vector<ItemInfo>::iterator it = vecSelectedItems.begin(); it != vecSelectedItems.end(); it++)
        {
            wstring strId = (*it).strOpcItemId;
            pTableItem = new QTableWidgetItem(QString::fromStdWString(strId));
            pTableItem->setFlags(Qt::ItemIsEnabled);
            ui.opcitemList->setItem(nRow, 0, pTableItem);
            vecServerHandle.push_back((*it).OpcItemSvrHandle);
            nRow++;
        }
        vector<VARIANT> vecValue;
        vector<LONG> vecErr;
        vector<LONG> vecQuality;
        vector<SYSTEMTIME> vecStamp;
        m_pOpcCtrl->SyncRead(nRow, vecServerHandle, vecValue, vecErr, vecQuality, vecStamp);
        for (int i = 0; i < nRow; i++)
        {
            pTableItem = new QTableWidgetItem(CommonTranslate::GetDataValue(vecValue[i]));
            pTableItem->setFlags(Qt::ItemIsEnabled);
            ui.opcitemList->setItem(i, 1, pTableItem);
            if (vecQuality.size() > i)
            {
                pTableItem = new QTableWidgetItem(QString("%1").arg(vecQuality[i]));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(i, 2, pTableItem);
            }
            if (vecStamp.size() > i)
            {
                pTableItem = new QTableWidgetItem(CommonTranslate::GetTimeStamp(vecStamp[i]));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(i, 3, pTableItem);
            }
            pTableItem = new QTableWidgetItem(QString("0x%1").arg(vecErr[i], 2, 16, QChar('0')));
            pTableItem->setFlags(Qt::ItemIsEnabled);
            ui.opcitemList->setItem(i, 4, pTableItem);
        }
        m_pOpcCtrl->SetOpcDataChange(this, &OPCServerDlg::ResponseDataChange);

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
    LONG serverHandle = curitem->data(0, Qt::UserRole).toInt();//ServerHander
    vector<LONG> vecRemove;
    vecRemove.push_back(serverHandle);
    m_pOpcCtrl->RemoveItems(vecRemove);
    GroupItem->removeChild(curitem);

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

void OPCServerDlg::on_OpcGrouptree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current->parent() == NULL)
        return;
    wstring strGroup = L"";
    if (current->childCount() == 0)
        strGroup = current->parent()->text(0).toStdWString();
    else
        strGroup = current->text(0).toStdWString();
    if (m_mapItems.size() <= 0 || strGroup == m_strSelectedGroup)
        return;
    m_strSelectedGroup = strGroup;
    map<wstring, vector<ItemInfo> >::iterator itfind = m_mapItems.find(m_strSelectedGroup);
    if (itfind != m_mapItems.end())
    {
        vector<ItemInfo>& vecItems = itfind->second;
        ui.opcitemList->setRowCount(vecItems.size());
        ui.btnEdit->setEnabled(true);
        vector<LONG> vecSvrHandle;
        int nRow = 0;
        QTableWidgetItem *pTableItem = NULL;
        for (vector<ItemInfo>::iterator it = vecItems.begin(); it != vecItems.end(); it++)
        {
            wstring strId = (*it).strOpcItemId;
            pTableItem = new QTableWidgetItem(QString::fromStdWString(strId));
            pTableItem->setFlags(Qt::ItemIsEnabled);
            ui.opcitemList->setItem(nRow, 0, pTableItem);
            vecSvrHandle.push_back((*it).OpcItemSvrHandle);
            nRow++;
        }
        vector<VARIANT> vecValue;
        vector<LONG> vecErr;
        vector<LONG> vecQuality;
        vector<SYSTEMTIME> vecStamp;
        m_pOpcCtrl->SyncRead(nRow, vecSvrHandle, vecValue, vecErr, vecQuality, vecStamp);
        for (int i = 0; i < nRow; i++)
        {
            pTableItem = new QTableWidgetItem(CommonTranslate::GetDataValue(vecValue[i]));
            pTableItem->setFlags(Qt::ItemIsEnabled);
            ui.opcitemList->setItem(i, 1, pTableItem);
            if (vecQuality.size() > i)
            {
                pTableItem = new QTableWidgetItem(QString("%1").arg(vecQuality[i]));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(i, 2, pTableItem);
            }
            if (vecStamp.size() > i)
            {
                pTableItem = new QTableWidgetItem(CommonTranslate::GetTimeStamp(vecStamp[i]));
                pTableItem->setFlags(Qt::ItemIsEnabled);
                ui.opcitemList->setItem(i, 3, pTableItem);
            }
            pTableItem = new QTableWidgetItem(QString("0x%1").arg(vecErr[i], 2, 16, QChar('0')));
            pTableItem->setFlags(Qt::ItemIsEnabled);
            ui.opcitemList->setItem(i, 4, pTableItem);
        }
    }
}

void OPCServerDlg::EventResponse_DataChange(long NumItems, const  vector<long>& vecClientHandle, const vector<VARIANT>& vecData, const vector<long>& vecQuality, const vector<SYSTEMTIME>& vecStamp)
{
    if (NumItems <= 0 || m_mapItems.size() <= 0 || m_bEdit)
        return;
    map<wstring, vector<ItemInfo> >::iterator itfind = m_mapItems.find(m_strSelectedGroup);
    if (itfind == m_mapItems.end())
        return;
    QTableWidgetItem *pItem = NULL;
    vector<ItemInfo>& vecSelectedItems = itfind->second;
    for (int i = 0; i < NumItems; i++)
    {
        int nRow = GetItemIndexInVector(vecClientHandle[i], vecSelectedItems);
        if (nRow >= 0)
        {
            pItem = ui.opcitemList->item(nRow, 1);
            if (pItem != NULL)
                pItem->setText(CommonTranslate::GetDataValue(vecData[i]));
            pItem = ui.opcitemList->item(nRow, 2);
            if (pItem != NULL)
                pItem->setText(QString("%1").arg(vecQuality[i]));
            pItem = ui.opcitemList->item(nRow, 3);
            if (pItem != NULL)
                pItem->setText(CommonTranslate::GetTimeStamp(vecStamp[i]));
        }
    }
}

void OPCServerDlg::GetOpcData(wstring& strSvrName, vector<GroupParam>& vecGroups, map<wstring, vector<ItemInfo> >& mapItems)
{
    strSvrName = m_strServerName;
    vecGroups = m_vecGroups;
    mapItems = m_mapItems;
}

void OPCServerDlg::on_opcitemList_itemChanged(QTableWidgetItem *item)
{
    if (item->column() != 1)
        return;
    wstring strData = item->text().toStdWString();//SyncWrite
}

void OPCServerDlg::on_btnEdit_clicked()
{
    int nCount = ui.opcitemList->rowCount();
    if (nCount <= 0)
        return;
    if (m_bEdit)
    {
        m_bEdit = false;
        //数据写入OPC

        ui.btnEdit->setText(QString::fromStdWString(L"编辑"));
    }
    else
    {
        m_bEdit = true;
        //QTableWidget和OPC Item数据缓存

        for (int i = 0; i < nCount; i++)
        {
            ui.opcitemList->item(i, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        }
        ui.btnEdit->setText(QString::fromStdWString(L"写入"));
    }
}