#include "stdafx.h"
#include "DefineNum.h"
#include "chinesetonum.h"

const int PAIR_COUNT = 8;
CHN_NAME_VALUE chnValuePair[] =
{
    { "ʰ", 10, false }, { "��", 100, false }, { "Ǫ", 1000, false },
    { "ʮ", 10, false }, { "��", 100, false }, { "ǧ", 1000, false },
    { "��", 10000, true }, { "��", 100000000, true }
};

const std::string chnRepeat = "��";

ChineseToNum::ChineseToNum()
{
}


ChineseToNum::~ChineseToNum()
{
}

int ChineseToNum::ChineseToValue(const std::string& chnString)
{
    int rtn = -1;
    for (int i = 0; i < CHN_NUM_CHAR_COUNT; i++)
    {
        if( (chnNumChar[i] == chnString) || (chnNumChar_s[i] == chnString))
        {
            rtn = i;
            break;
        }
    }
    return rtn;
}

int ChineseToNum::ChineseToUnit(const std::string& chnUnit, bool&  secUnit)
{
    int nRet = 0;
    for (int i = 0; i < PAIR_COUNT; i++)
    {
        if (chnUnit == chnValuePair[i].name)
        {
            nRet = chnValuePair[i].value;
            secUnit = chnValuePair[i].secUnit;
            break;
        }
    }
    return nRet;
}

bool ChineseToNum::IsRepeat(const std::string& strSection)
{
    if(strSection == chnRepeat)
    {
        return true;
    }
    return false;
}

long long ChineseToNum::ChnStrToNum(const std::string& chnString)
{
    long long rtn = 0;
    long long section = 0;
    int number = 0;
    bool secUnit = false;
    std::string::size_type pos = 0;
    while (pos < chnString.length())
    {
        int num = ChineseToValue(chnString.substr(pos, CHN_CHAR_LENGTH));
        if (num >= 0)//����or��λ��
        {
            number = num;
            pos += CHN_CHAR_LENGTH;
            if (pos >= chnString.length()) //���һλ���֣�ֱ�ӽ���
            {
                section += number;
                rtn += section;
                break;
            }
        }
        else
        {
            int unit = ChineseToUnit(chnString.substr(pos, CHN_CHAR_LENGTH), secUnit);
            //�ǽ�Ȩλ���Ҳ���"����""����"����˵��һ�����Ѿ�����
            if (secUnit && !IsRepeat(chnString.substr(pos+CHN_CHAR_LENGTH, CHN_CHAR_LENGTH)))
            {
                section = (section + number) * unit;
                rtn += section;
                section = 0;
            }
            else
            {
                section += (number * unit);
            }
            number = 0;
            pos += CHN_CHAR_LENGTH;
            if (pos >= chnString.length())
            {
                rtn += section;
                break;
            }
        }
    }
    return rtn;
}
