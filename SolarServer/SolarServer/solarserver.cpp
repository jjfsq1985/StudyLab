#include "stdafx.h"
#include "solarserver.h"

#include "OpcCtrl.h"
#include "opclistdlg.h"

SolarServer::SolarServer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    InitServer();
}

SolarServer::~SolarServer()
{
    if (m_pOpc != NULL)
        delete m_pOpc;
    m_pOpc = NULL;
}

void SolarServer::InitServer()
{
    m_pOpc = new OpcCtrl();

    //��ʼ������
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