#include "stdafx.h"
#include "DefineNum.h"
#include "numtochinese.h"

NumToChinese::NumToChinese()
{

}

NumToChinese::~NumToChinese()
{

}


void NumToChinese::NumberToChnStr(unsigned int num, std::string& chnStr)
{
  chnStr = "";
  int unitPos = 0;
  std::string strIns;
  bool needZero = false;
  while(num > 0)
  {
      unsigned int section = num % 10000;
      if(needZero)
      {
          chnStr.insert(0, chnNumChar[0]);
      }
      SectionToChinese(section, strIns);
      //是否需要节权位
      strIns += (section != 0) ? chnUnitSection[unitPos] : chnUnitSection[0];
      chnStr.insert(0,strIns);
      needZero = (section < 1000) && (section > 0);
      num = num / 10000;
      unitPos++;
  }
}

void NumToChinese::SectionToChinese(unsigned int section, std::string& chnStr)
{
    chnStr = "";
    int unitPos = 0;
    std::string strIns;
    bool zero = true;
    while(section > 0)
    {
        int v = section % 10;
        if(v == 0)
        {
            if ( (section == 0) || !zero )
            {
                zero  = true;//补零，zero作用是确保对连续多个0只补一个零
                chnStr.insert(0, chnNumChar[v]);
            }
        }
        else
        {
            zero = false;
            strIns = chnNumChar[v];//数字
            strIns += chnUnitChar[unitPos]; //权位
            chnStr.insert(0, strIns);
        }
        unitPos ++;
        section = section / 10;
    }
}

