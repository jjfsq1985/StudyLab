#include "stdafx.h"
#include "browseitemsdlg.h"
#include "OpcCtrl.h"

BrowseItemsDlg::BrowseItemsDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_pOpcCtrl = NULL;  
    ui.treeGroups->header()->hide();
}

BrowseItemsDlg::~BrowseItemsDlg()
{

}

void BrowseItemsDlg::setOpcCtrl(class OpcCtrl *pCtrl)
{
    m_pOpcCtrl = pCtrl;
}

void BrowseItemsDlg::browseItems()
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

void BrowseItemsDlg::on_treeGroups_itemExpanded(QTreeWidgetItem*Item)
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
        childItem->setData( 0, Qt::UserRole, QVariant(QString::fromStdWString(strData)) );
        Item->addChild(childItem);
    }
}

void BrowseItemsDlg::on_treeGroups_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current->childCount() > 0 || current->parent() == NULL)
        return;
    QString strItemId = current->data(0, Qt::UserRole).toString();
    wstring cItemID = strItemId.toStdWString();
    LONG nCount;
    vector<LONG> vecID;
    vector<LONG> vecType;
    vector<_bstr_t> vecDesc;
    m_pOpcCtrl->QueryItemProperties(cItemID.c_str(), nCount, vecID, vecDesc, vecType);
    vector<VARIANT> vecValue;
    vector<LONG> vecErr;
    m_pOpcCtrl->GetItemProperties(cItemID.c_str(), nCount, vecID, vecValue, vecErr);
    ui.tableItemProp->clearContents();
    ui.tableItemProp->setRowCount(nCount);
    for (int i = 0; i < nCount; i++)
    {
        QString strId = QString("%1").arg(vecID[i]);
        ui.tableItemProp->setItem(i, 0, new QTableWidgetItem(strId));
        wstring strDesc = vecDesc[i].GetBSTR();
        ui.tableItemProp->setItem( i, 1, new QTableWidgetItem(QString::fromStdWString(strDesc)) );
        ui.tableItemProp->setItem(i, 2, new QTableWidgetItem(m_pOpcCtrl->GetDataType(vecType[i])));
        ui.tableItemProp->setItem(i, 3, new QTableWidgetItem(m_pOpcCtrl->GetDataValue(vecValue[i])));
        ui.tableItemProp->setItem(i, 4, new QTableWidgetItem(QString::fromStdWString(cItemID)));
    }
}
