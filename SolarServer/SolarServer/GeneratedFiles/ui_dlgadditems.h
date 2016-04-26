/********************************************************************************
** Form generated from reading UI file 'dlgadditems.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGADDITEMS_H
#define UI_DLGADDITEMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_dlgAddItems
{
public:
    QTreeWidget *treeGroups;
    QTableWidget *tableItems;
    QPushButton *BtnAddItems;
    QPushButton *btnExit;

    void setupUi(QDialog *dlgAddItems)
    {
        if (dlgAddItems->objectName().isEmpty())
            dlgAddItems->setObjectName(QStringLiteral("dlgAddItems"));
        dlgAddItems->resize(621, 385);
        treeGroups = new QTreeWidget(dlgAddItems);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeGroups->setHeaderItem(__qtreewidgetitem);
        treeGroups->setObjectName(QStringLiteral("treeGroups"));
        treeGroups->setGeometry(QRect(0, 10, 271, 371));
        tableItems = new QTableWidget(dlgAddItems);
        if (tableItems->columnCount() < 2)
            tableItems->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableItems->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableItems->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableItems->setObjectName(QStringLiteral("tableItems"));
        tableItems->setGeometry(QRect(280, 10, 241, 371));
        tableItems->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableItems->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableItems->setSelectionBehavior(QAbstractItemView::SelectRows);
        BtnAddItems = new QPushButton(dlgAddItems);
        BtnAddItems->setObjectName(QStringLiteral("BtnAddItems"));
        BtnAddItems->setGeometry(QRect(540, 40, 75, 23));
        btnExit = new QPushButton(dlgAddItems);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setGeometry(QRect(540, 130, 75, 23));

        retranslateUi(dlgAddItems);
        QObject::connect(btnExit, SIGNAL(clicked()), dlgAddItems, SLOT(accept()));

        QMetaObject::connectSlotsByName(dlgAddItems);
    } // setupUi

    void retranslateUi(QDialog *dlgAddItems)
    {
        dlgAddItems->setWindowTitle(QApplication::translate("dlgAddItems", "dlgAddItems", 0));
        QTableWidgetItem *___qtablewidgetitem = tableItems->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("dlgAddItems", "Item ID", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableItems->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("dlgAddItems", "Result", 0));
        BtnAddItems->setText(QApplication::translate("dlgAddItems", "AddItems", 0));
        btnExit->setText(QApplication::translate("dlgAddItems", "Quit", 0));
    } // retranslateUi

};

namespace Ui {
    class dlgAddItems: public Ui_dlgAddItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGADDITEMS_H
