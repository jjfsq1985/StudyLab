#ifndef OPCLISTDLG_H
#define OPCLISTDLG_H

#include <QDialog>
#include "ui_opclistdlg.h"

#include <string>
#include <vector>
using namespace std;

class OPCListDlg : public QDialog
{
    Q_OBJECT

public:
    OPCListDlg(QWidget *parent = 0);
    ~OPCListDlg();

public:
    void setOpcCtrl(class OpcCtrl *pCtrl);
    void enumServer();
    wstring GetServerName();

    private slots:
        virtual void accept();

private:
    QString m_strServer;
    class OpcCtrl *m_pOpcCtrl;
private:
    Ui::DlgOpcList ui;
};

#endif // OPCLISTDLG_H
