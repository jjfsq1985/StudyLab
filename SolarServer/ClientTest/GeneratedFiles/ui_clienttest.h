/********************************************************************************
** Form generated from reading UI file 'clienttest.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTTEST_H
#define UI_CLIENTTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientTestClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ClientTestClass)
    {
        if (ClientTestClass->objectName().isEmpty())
            ClientTestClass->setObjectName(QStringLiteral("ClientTestClass"));
        ClientTestClass->resize(600, 400);
        menuBar = new QMenuBar(ClientTestClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ClientTestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ClientTestClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ClientTestClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ClientTestClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ClientTestClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ClientTestClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ClientTestClass->setStatusBar(statusBar);

        retranslateUi(ClientTestClass);

        QMetaObject::connectSlotsByName(ClientTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *ClientTestClass)
    {
        ClientTestClass->setWindowTitle(QApplication::translate("ClientTestClass", "ClientTest", 0));
    } // retranslateUi

};

namespace Ui {
    class ClientTestClass: public Ui_ClientTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTTEST_H
