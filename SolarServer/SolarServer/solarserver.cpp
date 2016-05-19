#include "stdafx.h"
#include "solarserver.h"

#include "OpcCtrl.h"
#include "opclistdlg.h"
#include "TcpServer.h"

SolarServer::SolarServer(QWidget *parent)
    : QMainWindow(parent)
{
    m_pOpc = NULL;
    m_pTcp = NULL;
    ui.setupUi(this);

    InitServer();
}

SolarServer::~SolarServer()
{
    if (m_pOpc != NULL)
        delete m_pOpc;
    m_pOpc = NULL;

    if (m_pTcp != NULL)
        delete m_pTcp;
    m_pTcp = NULL;
}

void SolarServer::InitServer()
{
    m_pOpc = new OpcCtrl();

    //³õÊ¼»¯ÍøÂç
    m_pTcp = new TcpServer();
    m_pTcp->Init(9999);
}

void SolarServer::on_actionOPC_Server_triggered()
{
    OPCServerDlg dlg(this);
    dlg.setOpcCtrl(m_pOpc);
    dlg.InitOpcServer(m_strSvrName, m_vecGroups, m_mapItems);
    if (dlg.exec() == QDialog::Accepted)
    {
      dlg.GetOpcData(m_strSvrName, m_vecGroups, m_mapItems);
    }
}