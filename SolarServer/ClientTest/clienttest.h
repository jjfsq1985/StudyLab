#ifndef CLIENTTEST_H
#define CLIENTTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_clienttest.h"

class ClientTest : public QMainWindow
{
    Q_OBJECT

public:
    ClientTest(QWidget *parent = 0);
    ~ClientTest();

private:
    Ui::ClientTestClass ui;

private:
    class TcpClient *m_pClient;
};

#endif // CLIENTTEST_H
