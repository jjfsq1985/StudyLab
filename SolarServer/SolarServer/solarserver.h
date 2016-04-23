#ifndef SOLARSERVER_H
#define SOLARSERVER_H

#include <QtWidgets/QMainWindow>
#include "ui_solarserver.h"

class SolarServer : public QMainWindow
{
    Q_OBJECT

public:
    SolarServer(QWidget *parent = 0);
    ~SolarServer();

private slots:
    void on_actionOPC_Server_triggered();

private:
    void InitServer();

private:
    Ui::SolarServerClass ui;

private:
    class OpcCtrl *m_pOpc;
};

#endif // SOLARSERVER_H
