/********************************************************************************
** Form generated from reading UI file 'opcserverdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPCSERVERDLG_H
#define UI_OPCSERVERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgOpcServer
{
public:
    QTreeWidget *OpcGrouptree;
    QTableWidget *opcitemList;
    QDialogButtonBox *buttonBox;
    QPushButton *btnconnect;
    QPushButton *btnDisconnect;
    QPushButton *btnEdit;

    void setupUi(QDialog *DlgOpcServer)
    {
        if (DlgOpcServer->objectName().isEmpty())
            DlgOpcServer->setObjectName(QStringLiteral("DlgOpcServer"));
        DlgOpcServer->resize(940, 440);
        OpcGrouptree = new QTreeWidget(DlgOpcServer);
        OpcGrouptree->setObjectName(QStringLiteral("OpcGrouptree"));
        OpcGrouptree->setGeometry(QRect(9, 40, 241, 391));
        opcitemList = new QTableWidget(DlgOpcServer);
        if (opcitemList->columnCount() < 6)
            opcitemList->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        opcitemList->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        opcitemList->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        opcitemList->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        opcitemList->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        opcitemList->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        opcitemList->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        opcitemList->setObjectName(QStringLiteral("opcitemList"));
        opcitemList->setGeometry(QRect(260, 40, 671, 351));
        buttonBox = new QDialogButtonBox(DlgOpcServer);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(760, 400, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        btnconnect = new QPushButton(DlgOpcServer);
        btnconnect->setObjectName(QStringLiteral("btnconnect"));
        btnconnect->setGeometry(QRect(30, 10, 75, 23));
        btnDisconnect = new QPushButton(DlgOpcServer);
        btnDisconnect->setObjectName(QStringLiteral("btnDisconnect"));
        btnDisconnect->setGeometry(QRect(130, 10, 75, 23));
        btnEdit = new QPushButton(DlgOpcServer);
        btnEdit->setObjectName(QStringLiteral("btnEdit"));
        btnEdit->setGeometry(QRect(490, 400, 75, 23));

        retranslateUi(DlgOpcServer);
        QObject::connect(buttonBox, SIGNAL(accepted()), DlgOpcServer, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DlgOpcServer, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgOpcServer);
    } // setupUi

    void retranslateUi(QDialog *DlgOpcServer)
    {
        DlgOpcServer->setWindowTitle(QApplication::translate("DlgOpcServer", "Dialog", 0));
        QTreeWidgetItem *___qtreewidgetitem = OpcGrouptree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("DlgOpcServer", "Groups", 0));
        QTableWidgetItem *___qtablewidgetitem = opcitemList->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DlgOpcServer", "ItemID", 0));
        QTableWidgetItem *___qtablewidgetitem1 = opcitemList->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DlgOpcServer", "AccessRight", 0));
        QTableWidgetItem *___qtablewidgetitem2 = opcitemList->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("DlgOpcServer", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem3 = opcitemList->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("DlgOpcServer", "Quality", 0));
        QTableWidgetItem *___qtablewidgetitem4 = opcitemList->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("DlgOpcServer", "Time Stamp", 0));
        QTableWidgetItem *___qtablewidgetitem5 = opcitemList->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("DlgOpcServer", "Result", 0));
        btnconnect->setText(QApplication::translate("DlgOpcServer", "Connect", 0));
        btnDisconnect->setText(QApplication::translate("DlgOpcServer", "Disconnect", 0));
        btnEdit->setText(QApplication::translate("DlgOpcServer", "\347\274\226\350\276\221", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgOpcServer: public Ui_DlgOpcServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPCSERVERDLG_H
