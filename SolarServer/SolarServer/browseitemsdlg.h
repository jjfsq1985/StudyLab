#ifndef BROWSEITEMSDLG_H
#define BROWSEITEMSDLG_H

#include <QDialog>
#include "ui_browseitemsdlg.h"

class BrowseItemsDlg : public QDialog
{
    Q_OBJECT

public:
    BrowseItemsDlg(QWidget *parent = 0);
    ~BrowseItemsDlg();

public:
    void setOpcCtrl(class OpcCtrl *pCtrl);
    void browseItems();

    private slots:
    void on_treeGroups_itemExpanded(QTreeWidgetItem*Item);
    void on_treeGroups_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    class OpcCtrl *m_pOpcCtrl;

private:
    Ui::BrowseItemsDlg ui;
};

#endif // BROWSEITEMSDLG_H
