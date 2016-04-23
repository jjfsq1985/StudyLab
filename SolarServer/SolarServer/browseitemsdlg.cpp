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
        ui.tableItemProp->setItem( i, 2, new QTableWidgetItem(GetDataType(vecType[i])) );
        ui.tableItemProp->setItem(i, 3, new QTableWidgetItem(GetDataValue(vecValue[i])));
        ui.tableItemProp->setItem(i, 4, new QTableWidgetItem(QString::fromStdWString(cItemID)));
    }
}

QString BrowseItemsDlg::GetDataType(LONG dataType)
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

QString BrowseItemsDlg::GetDataValue(VARIANT varValue)
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