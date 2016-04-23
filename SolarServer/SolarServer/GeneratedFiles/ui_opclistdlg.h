/********************************************************************************
** Form generated from reading UI file 'opclistdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPCLISTDLG_H
#define UI_OPCLISTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DlgOpcList
{
public:
    QListWidget *listWidget;
    QLabel *label;
    QPushButton *OPCconnect;

    void setupUi(QDialog *DlgOpcList)
    {
        if (DlgOpcList->objectName().isEmpty())
            DlgOpcList->setObjectName(QStringLiteral("DlgOpcList"));
        DlgOpcList->resize(400, 300);
        listWidget = new QListWidget(DlgOpcList);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(0, 40, 391, 211));
        label = new QLabel(DlgOpcList);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 54, 12));
        OPCconnect = new QPushButton(DlgOpcList);
        OPCconnect->setObjectName(QStringLiteral("OPCconnect"));
        OPCconnect->setGeometry(QRect(140, 260, 75, 23));

        retranslateUi(DlgOpcList);
        QObject::connect(OPCconnect, SIGNAL(clicked()), DlgOpcList, SLOT(accept()));
        QObject::connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), DlgOpcList, SLOT(accept()));

        QMetaObject::connectSlotsByName(DlgOpcList);
    } // setupUi

    void retranslateUi(QDialog *DlgOpcList)
    {
        DlgOpcList->setWindowTitle(QApplication::translate("DlgOpcList", "OPCListDlg", 0));
        label->setText(QApplication::translate("DlgOpcList", "OpcServer", 0));
        OPCconnect->setText(QApplication::translate("DlgOpcList", "\350\277\236\346\216\245", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgOpcList: public Ui_DlgOpcList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPCLISTDLG_H
