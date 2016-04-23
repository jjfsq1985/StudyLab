/********************************************************************************
** Form generated from reading UI file 'browseitemsdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BROWSEITEMSDLG_H
#define UI_BROWSEITEMSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_BrowseItemsDlg
{
public:
    QTableWidget *tableItemProp;
    QTreeWidget *treeGroups;

    void setupUi(QDialog *BrowseItemsDlg)
    {
        if (BrowseItemsDlg->objectName().isEmpty())
            BrowseItemsDlg->setObjectName(QStringLiteral("BrowseItemsDlg"));
        BrowseItemsDlg->resize(767, 384);
        tableItemProp = new QTableWidget(BrowseItemsDlg);
        if (tableItemProp->columnCount() < 5)
            tableItemProp->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableItemProp->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableItemProp->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableItemProp->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableItemProp->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableItemProp->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableItemProp->setObjectName(QStringLiteral("tableItemProp"));
        tableItemProp->setGeometry(QRect(240, 10, 521, 361));
        treeGroups = new QTreeWidget(BrowseItemsDlg);
        treeGroups->setObjectName(QStringLiteral("treeGroups"));
        treeGroups->setGeometry(QRect(10, 10, 221, 361));
        QWidget::setTabOrder(treeGroups, tableItemProp);

        retranslateUi(BrowseItemsDlg);

        QMetaObject::connectSlotsByName(BrowseItemsDlg);
    } // setupUi

    void retranslateUi(QDialog *BrowseItemsDlg)
    {
        BrowseItemsDlg->setWindowTitle(QApplication::translate("BrowseItemsDlg", "BrowseItemsDlg", 0));
        QTableWidgetItem *___qtablewidgetitem = tableItemProp->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("BrowseItemsDlg", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableItemProp->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("BrowseItemsDlg", "Describe", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableItemProp->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("BrowseItemsDlg", "DataType", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableItemProp->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("BrowseItemsDlg", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableItemProp->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("BrowseItemsDlg", "ItemID", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeGroups->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("BrowseItemsDlg", "Groups", 0));
    } // retranslateUi

};

namespace Ui {
    class BrowseItemsDlg: public Ui_BrowseItemsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWSEITEMSDLG_H
