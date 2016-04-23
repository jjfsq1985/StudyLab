/********************************************************************************
** Form generated from reading UI file 'dlgaddgroup.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGADDGROUP_H
#define UI_DLGADDGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DlgAddGroup
{
public:
    QLabel *label;
    QTextEdit *groupName;
    QCheckBox *Actived;
    QLabel *label_2;
    QSpinBox *updateRate;
    QPushButton *btnOk;

    void setupUi(QDialog *DlgAddGroup)
    {
        if (DlgAddGroup->objectName().isEmpty())
            DlgAddGroup->setObjectName(QStringLiteral("DlgAddGroup"));
        DlgAddGroup->resize(396, 171);
        label = new QLabel(DlgAddGroup);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 20, 61, 16));
        groupName = new QTextEdit(DlgAddGroup);
        groupName->setObjectName(QStringLiteral("groupName"));
        groupName->setGeometry(QRect(120, 20, 191, 21));
        groupName->setInputMethodHints(Qt::ImhNone);
        groupName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        groupName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        Actived = new QCheckBox(DlgAddGroup);
        Actived->setObjectName(QStringLiteral("Actived"));
        Actived->setGeometry(QRect(110, 60, 71, 16));
        label_2 = new QLabel(DlgAddGroup);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 90, 71, 16));
        updateRate = new QSpinBox(DlgAddGroup);
        updateRate->setObjectName(QStringLiteral("updateRate"));
        updateRate->setGeometry(QRect(130, 90, 131, 22));
        updateRate->setMinimum(100);
        updateRate->setMaximum(5000);
        updateRate->setSingleStep(10);
        updateRate->setValue(1000);
        btnOk = new QPushButton(DlgAddGroup);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setGeometry(QRect(140, 130, 75, 23));

        retranslateUi(DlgAddGroup);
        QObject::connect(btnOk, SIGNAL(clicked()), DlgAddGroup, SLOT(accept()));

        QMetaObject::connectSlotsByName(DlgAddGroup);
    } // setupUi

    void retranslateUi(QDialog *DlgAddGroup)
    {
        DlgAddGroup->setWindowTitle(QApplication::translate("DlgAddGroup", "DlgAddGroup", 0));
        label->setText(QApplication::translate("DlgAddGroup", "Group Name", 0));
        Actived->setText(QApplication::translate("DlgAddGroup", "Actived", 0));
        label_2->setText(QApplication::translate("DlgAddGroup", "Update Rate", 0));
        btnOk->setText(QApplication::translate("DlgAddGroup", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgAddGroup: public Ui_DlgAddGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGADDGROUP_H
