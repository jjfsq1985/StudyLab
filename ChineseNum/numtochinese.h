#ifndef NUMTOCHINESE_H
#define NUMTOCHINESE_H
#include <string>

class NumToChinese
{
public:
    NumToChinese();
    ~NumToChinese();

public:
    void NumberToChnStr(long long num, std::string& chnStr);

private:
    void SectionToChinese(unsigned int section, std::string& chnStr);

};

#endif // NUMTOCHINESE_H
