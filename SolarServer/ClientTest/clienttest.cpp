#include "stdafx.h"
#include "clienttest.h"
#include "TcpClient.h"

ClientTest::ClientTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_pClient = new TcpClient();
    m_pClient->Init("192.168.1.123", 9999);
}

ClientTest::~ClientTest()
{
    if (m_pClient != NULL)
        delete m_pClient;
    m_pClient = NULL;
}
