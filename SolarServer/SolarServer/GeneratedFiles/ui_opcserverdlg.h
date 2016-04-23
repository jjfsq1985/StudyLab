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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgOpcServer
{
public:
    QTreeWidget *OpcGrouptree;
    QListWidget *opcitemList;
    QDialogButtonBox *buttonBox;
    QPushButton *btnconnect;
    QPushButton *btnDisconnect;

    void setupUi(QDialog *DlgOpcServer)
    {
        if (DlgOpcServer->objectName().isEmpty())
            DlgOpcServer->setObjectName(QStringLiteral("DlgOpcServer"));
        DlgOpcServer->resize(809, 396);
        OpcGrouptree = new QTreeWidget(DlgOpcServer);
        OpcGrouptree->setObjectName(QStringLiteral("OpcGrouptree"));
        OpcGrouptree->setGeometry(QRect(9, 40, 271, 351));
        opcitemList = new QListWidget(DlgOpcServer);
        opcitemList->setObjectName(QStringLiteral("opcitemList"));
        opcitemList->setGeometry(QRect(300, 30, 491, 261));
        buttonBox = new QDialogButtonBox(DlgOpcServer);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(590, 340, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        btnconnect = new QPushButton(DlgOpcServer);
        btnconnect->setObjectName(QStringLiteral("btnconnect"));
        btnconnect->setGeometry(QRect(30, 10, 75, 23));
        btnDisconnect = new QPushButton(DlgOpcServer);
        btnDisconnect->setObjectName(QStringLiteral("btnDisconnect"));
        btnDisconnect->setGeometry(QRect(130, 10, 75, 23));

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
        btnconnect->setText(QApplication::translate("DlgOpcServer", "Connect", 0));
        btnDisconnect->setText(QApplication::translate("DlgOpcServer", "Disconnect", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgOpcServer: public Ui_DlgOpcServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPCSERVERDLG_H
