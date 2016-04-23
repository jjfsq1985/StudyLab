#include "stdafx.h"
#include "opclistdlg.h"
#include "OpcCtrl.h"
#include  <QString>


OPCListDlg::OPCListDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_pOpcCtrl = NULL;
    m_strServer = "";
}

OPCListDlg::~OPCListDlg()
{

}

void OPCListDlg::setOpcCtrl(class OpcCtrl *pCtrl)
{
    m_pOpcCtrl = pCtrl;
}

void OPCListDlg::enumServer()
{
    vector<_bstr_t> vecAllServer;
    m_pOpcCtrl->getListOfServers("localhost", vecAllServer);
    if (vecAllServer.size() <= 0)
        return;
    for (vector<_bstr_t>::iterator it = vecAllServer.begin(); it != vecAllServer.end(); it++)
    {
        wstring str = (*it).GetBSTR();
        QString strItem = QString::fromStdWString(str);
        ui.listWidget->addItem(strItem);
    }
}

void OPCListDlg::accept()
{
    m_strServer = ui.listWidget->currentItem()->text();
    done(Accepted);
}

wstring OPCListDlg::GetServerName()
{
    return m_strServer.toStdWString();
}