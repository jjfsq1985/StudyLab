#ifndef CHINESETONUM_H
#define CHINESETONUM_H
#include <string>

typedef struct  
{
    const char* name; //����Ȩλ����
    int value;//10�ı���ֵ
    bool secUnit; //�Ƿ��Ȩλ
}CHN_NAME_VALUE;



class ChineseToNum
{
public:
    ChineseToNum();
    ~ChineseToNum();

public:
    unsigned int ChnStrToNum(const std::string& chnString);

private:
    int ChineseToValue(const std::string& chnString);
    int ChineseToUnit(const std::string& chnUnit, bool&  secUnit);
};

#endif