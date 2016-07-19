#include "stdafx.h"
#include "numtochinese.h"
#include "chinesetonum.h"
#include <string>

int main(int argc, char* argv[])
{
    NumToChinese numtochn;
    unsigned int data = 32453245;
    std::string output;
    numtochn.NumberToChnStr(data,output);

    ChineseToNum numTrans;
    unsigned int outNum = numTrans.ChnStrToNum(output);


    printf("%d is %s\r\n",data, output.c_str());
    printf("trans %d\r\n", outNum);
}
