#include "stdafx.h"
#include "solarserver.h"

#include "OpcCtrl.h"
#include "opcserverdlg.h"
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

    //³õÊ¼»¯ÍøÂç
}

void SolarServer::on_actionOPC_Server_triggered()
{
    OPCServerDlg dlg(this);
    dlg.setOpcCtrl(m_pOpc);
    dlg.exec();
}