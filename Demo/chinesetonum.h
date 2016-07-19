#ifndef CHINESETONUM_H
#define CHINESETONUM_H
#include <string>

typedef struct  
{
    const char* name; //中文权位名称
    int value;//10的倍数值
    bool secUnit; //是否节权位
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