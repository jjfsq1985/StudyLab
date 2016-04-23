#include "stdafx.h"
#include "dlgaddgroup.h"

DlgAddGroup::DlgAddGroup(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ui.Actived->setChecked(true);
}

DlgAddGroup::~DlgAddGroup()
{

}

void DlgAddGroup::SetAddedGroup(const vector<GroupParam>& vecAddedGroup)
{
    m_vecGroups = vecAddedGroup;
}

void DlgAddGroup::accept()
{
    wstring strText = ui.groupName->toPlainText().toStdWString();
    bool bRet = true;
    for (vector<GroupParam>::iterator it = m_vecGroups.begin(); it != m_vecGroups.end(); it++)
    {
        if ((*it).strGroupName == strText)
        {
            bRet = false;
            break;
        }
    }

    if (bRet)
    {
        m_strGroupName = strText;
        m_bActived = ui.Actived->isChecked();
        m_nUpdateRate = ui.updateRate->value();
        done(Accepted);
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Group Name Has Existed");
    }
}

void DlgAddGroup::GetGroupParam(wstring& strGroupName, bool& bActived, int& nUpdateRate)
{
    strGroupName = m_strGroupName;
    bActived = m_bActived;
    nUpdateRate = m_nUpdateRate;
}