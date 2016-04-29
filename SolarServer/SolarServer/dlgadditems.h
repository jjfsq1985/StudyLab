#ifndef DLGADDITEMS_H
#define DLGADDITEMS_H

#include <QDialog>
#include "ui_dlgadditems.h"
#include <vector>
#include <string>
using namespace std;

typedef struct _tagItemInfo
{
    wstring strOpcItemId;
    LONG OpcItemSvrHandle;
    LONG OpcItemClientHandle;
    LONG AccessRight;
}ItemInfo;

class dlgAddItems : public QDialog
{
    Q_OBJECT

public:
    dlgAddItems(QWidget *parent = 0);
    ~dlgAddItems();

public:
    void setOpcCtrl(class OpcCtrl *pCtrl);

    void browseItems();
    void GetSelectedItems(vector<ItemInfo>& vecSelectedItem);

    private slots:
    virtual void accept();
    void on_tableItems_customContextMenuRequested(QPoint pos);
    void on_treeGroups_itemExpanded(QTreeWidgetItem*Item);
    void on_treeGroups_itemDoubleClicked(QTreeWidgetItem*Item, int column);
    void on_tableItems_itemDoubleClicked(QTableWidgetItem*Item);
    void on_BtnAddItems_clicked();
    void RemoveItems();

private:    
    LONG GetServerHandle(wstring strItemId);
    vector<ItemInfo> m_vecItems;
    class OpcCtrl *m_pOpcCtrl;
private:
    QMenu *m_pContextMenu;
    QAction *m_pRemoveItems;
    Ui::dlgAddItems ui;
};

#endif // DLGADDITEMS_H
