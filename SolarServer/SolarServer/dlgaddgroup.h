#ifndef DLGADDGROUP_H
#define DLGADDGROUP_H

#include <QDialog>
#include "ui_dlgaddgroup.h"
#include <vector>
#include <string>
using namespace std;

typedef struct _tagGroupParam
{
    wstring strGroupName;
    bool bActive;
    int nUpdateRate;
}GroupParam;

class DlgAddGroup : public QDialog
{
    Q_OBJECT

public:
    DlgAddGroup(QWidget *parent = 0);
    ~DlgAddGroup();

public:
    void SetAddedGroup(const vector<GroupParam>& vecAddedGroup);
    void GetGroupParam(wstring& strGroupName, bool& bActived, int& nUpdateRate);

    private slots:
    virtual void accept();
    
private:
    vector<GroupParam> m_vecGroups;
    wstring m_strGroupName;
    bool m_bActived;
    int m_nUpdateRate;


private:
    Ui::DlgAddGroup ui;
};

#endif // DLGADDGROUP_H
