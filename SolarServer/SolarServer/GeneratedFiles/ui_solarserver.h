/********************************************************************************
** Form generated from reading UI file 'solarserver.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLARSERVER_H
#define UI_SOLARSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SolarServerClass
{
public:
    QAction *actionQuit;
    QAction *actionOPC_Server;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuOPC;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SolarServerClass)
    {
        if (SolarServerClass->objectName().isEmpty())
            SolarServerClass->setObjectName(QStringLiteral("SolarServerClass"));
        SolarServerClass->resize(600, 400);
        actionQuit = new QAction(SolarServerClass);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionOPC_Server = new QAction(SolarServerClass);
        actionOPC_Server->setObjectName(QStringLiteral("actionOPC_Server"));
        centralWidget = new QWidget(SolarServerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SolarServerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SolarServerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menuOPC = new QMenu(menuBar);
        menuOPC->setObjectName(QStringLiteral("menuOPC"));
        SolarServerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SolarServerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SolarServerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SolarServerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SolarServerClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuOPC->menuAction());
        menu->addAction(actionQuit);
        menuOPC->addAction(actionOPC_Server);

        retranslateUi(SolarServerClass);
        QObject::connect(actionQuit, SIGNAL(triggered()), SolarServerClass, SLOT(close()));

        QMetaObject::connectSlotsByName(SolarServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *SolarServerClass)
    {
        SolarServerClass->setWindowTitle(QApplication::translate("SolarServerClass", "SolarServer", 0));
        actionQuit->setText(QApplication::translate("SolarServerClass", "\351\200\200\345\207\272", 0));
        actionOPC_Server->setText(QApplication::translate("SolarServerClass", "OPC Server", 0));
        menu->setTitle(QApplication::translate("SolarServerClass", "\346\226\207\344\273\266", 0));
        menuOPC->setTitle(QApplication::translate("SolarServerClass", "OPC", 0));
    } // retranslateUi

};

namespace Ui {
    class SolarServerClass: public Ui_SolarServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLARSERVER_H
