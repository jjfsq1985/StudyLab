#ifndef OPCSERVERDLG_H
#define OPCSERVERDLG_H

#include <QDialog>
#include "ui_opcserverdlg.h"

#include "dlgaddgroup.h"

class OPCServerDlg : public QDialog
{
    Q_OBJECT

public:
    OPCServerDlg(QWidget *parent = 0);
    ~OPCServerDlg();

public:
    void setOpcCtrl(class OpcCtrl *pCtrl);

private:
    virtual void contextMenuEvent(QContextMenuEvent *e);

    private slots:
    void on_btnconnect_clicked();
    void on_btnDisconnect_clicked();
    void AddGroup();
    void BrowserItems();
    void DelGroup();
    void AddItems();

private:
    vector<GroupParam> m_vecGroups;
    class OpcCtrl *m_pOpcCtrl;
private:
    QMenu *m_pContextMenu;
    QAction *m_pAddGroup;
    QAction *m_pBrowserItems;
    QAction *m_pDelGroup;
    QAction *m_pAddItems;
    Ui::DlgOpcServer ui;
};

#endif // OPCSERVERDLG_H
