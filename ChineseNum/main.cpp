#include "stdafx.h"
#include "numtochinese.h"
#include "chinesetonum.h"
#include <string>

int main(int argc, char* argv[])
{
    NumToChinese numtochn;
    long long data = 1234532453245;
    std::string output;
    numtochn.NumberToChnStr(data,output);

    ChineseToNum numTrans;
    long long outNum = numTrans.ChnStrToNum(output);


    printf("%lld is %s\r\n",data, output.c_str());
    printf("trans %lld\r\n", outNum);
}
