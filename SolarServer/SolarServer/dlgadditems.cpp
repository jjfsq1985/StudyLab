#include "stdafx.h"
#include "dlgadditems.h"
#include <algorithm>
#include "OpcCtrl.h"

dlgAddItems::dlgAddItems(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_pOpcCtrl = NULL;
    ui.treeGroups->header()->hide();
    ui.tableItems->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pContextMenu = new QMenu(ui.tableItems);
    m_pRemoveItems = new QAction(tr("Remove Items..."), this);
    connect(m_pRemoveItems, &QAction::triggered, this, &dlgAddItems::RemoveItems);

}

dlgAddItems::~dlgAddItems()
{

}

void dlgAddItems::setOpcCtrl(class OpcCtrl *pCtrl)
{
    m_pOpcCtrl = pCtrl;
}

void dlgAddItems::browseItems()
{
    vector<OpcItem > vecAllBranches;
    m_pOpcCtrl->BrowserBranches(vecAllBranches);

    for (vector<OpcItem>::iterator it = vecAllBranches.begin(); it != vecAllBranches.end(); it++)
    {
        QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui.treeGroups);
        wstring strGroup = (*it).sItem.GetBSTR();
        rootItem->setText(0, QString::fromStdWString(strGroup));
        rootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        ui.treeGroups->addTopLevelItem(rootItem);
    }
}

void dlgAddItems::on_treeGroups_itemExpanded(QTreeWidgetItem*Item)
{
    if (Item->childCount() > 0)
        return;
    vector<OpcItem > vecLeafs;
    wstring strName = Item->text(0).toStdWString();
    m_pOpcCtrl->BrowserLeafs(strName.c_str(), vecLeafs);
    for (vector<OpcItem>::iterator it = vecLeafs.begin(); it != vecLeafs.end(); it++)
    {
        QTreeWidgetItem *childItem = new QTreeWidgetItem(Item);
        wstring strItem = (*it).sItem.GetBSTR();
        wstring strData = (*it).sItemId.GetBSTR();
        childItem->setText(0, QString::fromStdWString(strItem));
        childItem->setData(0, Qt::UserRole, QVariant(QString::fromStdWString(strData)));
        Item->addChild(childItem);
    }
}

void dlgAddItems::on_treeGroups_itemDoubleClicked(QTreeWidgetItem*Item, int column)
{
    if (Item->childCount() > 0 || Item->parent() == NULL)
        return;
    QString strItemId = Item->data(0, Qt::UserRole).toString();
    wstring cItemID = strItemId.toStdWString();
    bool bFind = false;
    for (vector<ItemInfo>::iterator it = m_vecItems.begin(); it != m_vecItems.end(); it++)
    {
        if ((*it).strOpcItemId == cItemID)
        {
            bFind = true;
            break;
        }
    }
    if (!bFind)
    {
        int nRow = ui.tableItems->rowCount();
        ui.tableItems->setRowCount(nRow + 1);
        QTableWidgetItem *pTableItem = new QTableWidgetItem(QString::fromStdWString(cItemID));
        ui.tableItems->setItem(nRow, 0, pTableItem);
        ItemInfo itemopc = { cItemID, 0 };
        m_vecItems.push_back(itemopc);
    }
}

LONG dlgAddItems::GetServerHandle(wstring strItemId)
{
    LONG handle = 0;
    for (vector<ItemInfo>::iterator it = m_vecItems.begin(); it != m_vecItems.end(); it++)
    {
        if (strItemId == (*it).strOpcItemId)
        {
            handle = (*it).OpcItemSvrHandle;
            break;
        }
    }
    return handle;
}

void dlgAddItems::on_tableItems_itemDoubleClicked(QTableWidgetItem*Item)
{
    if (Item == NULL)
        return;
    wstring strItem = Item->text().toStdWString();
    LONG serverHandle = GetServerHandle(strItem);
    if (serverHandle != 0)
    {
        //已增加需删除
        vector<LONG> vecRemove;
        vecRemove.push_back(serverHandle);
        m_pOpcCtrl->RemoveItems(vecRemove);
    }
    int nCount = m_vecItems.size();
    for (int i= 0; i < nCount; i++)
    {
        if (m_vecItems[i].strOpcItemId == strItem)
        {
            ui.tableItems->removeRow(Item->row());
            m_vecItems.erase(m_vecItems.begin() + i);
            break;
        }
    }
}

void dlgAddItems::on_tableItems_customContextMenuRequested(QPoint pos)
{
    m_pContextMenu->clear();
    m_pContextMenu->addAction(m_pRemoveItems);
    m_pContextMenu->exec(pos);
}

void dlgAddItems::RemoveItems()
{
    QList<QTableWidgetItem*> listDataItems = ui.tableItems->selectedItems();
    if (listDataItems.size() <= 0)
        return;
    vector<LONG> vecRemove;
    for (int i = 0; i < listDataItems.size(); i++)
    {
        wstring strItemId = listDataItems[i]->text().toStdWString();
        LONG serverHandle = GetServerHandle(strItemId);
        if (serverHandle != 0)
            vecRemove.push_back(serverHandle);
    }
    if (vecRemove.size() > 0)
        m_pOpcCtrl->RemoveItems(vecRemove); //已增加需删除
    for (int tableIndex = 0; tableIndex < listDataItems.size(); tableIndex++)
    {
        wstring strItemId = listDataItems[tableIndex]->text().toStdWString();

        for (int iVec = 0; iVec < m_vecItems.size(); iVec++)
        {
            if (m_vecItems[iVec].strOpcItemId == strItemId)
            {
                ui.tableItems->removeRow(listDataItems[tableIndex]->row());
                m_vecItems.erase(m_vecItems.begin() + iVec);
                break;
            }
        }
    }

}

void dlgAddItems::on_BtnAddItems_clicked()
{
    if (m_vecItems.size() <= 0)
        return;
    vector<int> vecAddItems;
    int nIndex = 0;
    for (vector<ItemInfo>::iterator it = m_vecItems.begin(); it != m_vecItems.end(); it++)
    {
        if ((*it).OpcItemSvrHandle == 0)
        {
            vecAddItems.push_back(nIndex);
        }
        nIndex++;
    }
    if (vecAddItems.size() <= 0)
        return;
    if (vecAddItems.size() == 1)
    {
        LONG ServerHandle = 0;
        LONG ClientHandle = 0;
        LONG lResult = 0;
        int nIndex = vecAddItems[0];
        if (m_pOpcCtrl->AddOpcItem(_bstr_t(m_vecItems[nIndex].strOpcItemId.c_str()), ServerHandle, ClientHandle, lResult))
        {
            m_vecItems[nIndex].OpcItemSvrHandle = ServerHandle;
            m_vecItems[nIndex].OpcItemClientHandle = ClientHandle;
            if (SUCCEEDED(lResult))
            {
                QTableWidgetItem *pTableItem = new QTableWidgetItem("OK");
                ui.tableItems->setItem(nIndex, 1, pTableItem);
            }
            else
            {
                QTableWidgetItem *pTableItem = new QTableWidgetItem(QString("0x%1").arg(lResult, 0, 16));
                ui.tableItems->setItem(nIndex, 1, pTableItem);
            }
        }
    }
    else
    {
        vector<_bstr_t> vecAddItemsID;
        for (int i = 0; i < vecAddItems.size(); i++)
        {
            wstring strItemId = m_vecItems[i].strOpcItemId;
            _bstr_t item(strItemId.c_str());
            vecAddItemsID.push_back(item);
        }
        vector<LONG> vecSvrHandle;
        vector<LONG> vecClientHandle;
        vector<LONG> vecResult;
        if (m_pOpcCtrl->AddOpcItems(vecAddItemsID, vecSvrHandle, vecClientHandle, vecResult))
        {
            for (int iAdd = 0; iAdd < vecSvrHandle.size(); iAdd++)
            {
                int nIndex = vecAddItems[iAdd];
                m_vecItems[nIndex].OpcItemSvrHandle = vecSvrHandle[iAdd];
                m_vecItems[nIndex].OpcItemClientHandle = vecClientHandle[iAdd];
                if (SUCCEEDED(vecResult[iAdd]))
                {
                    QTableWidgetItem *pTableItem = new QTableWidgetItem("OK");
                    ui.tableItems->setItem(nIndex, 1, pTableItem);
                }
                else
                {
                    QTableWidgetItem *pTableItem = new QTableWidgetItem(QString("0x%1").arg(vecResult[iAdd], 0, 16));
                    ui.tableItems->setItem(nIndex, 1, pTableItem);
                }
            }
        }
    }
}

void dlgAddItems::GetSelectedItems(vector<ItemInfo>& vecSelectedItem)
{
    for (vector<ItemInfo>::iterator it = m_vecItems.begin(); it != m_vecItems.end(); it++)
    {
        if ((*it).OpcItemSvrHandle != 0)
            vecSelectedItem.push_back(*it);
    }
}

void dlgAddItems::accept()
{
    done(Accepted);
}