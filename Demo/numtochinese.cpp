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
      //�Ƿ���Ҫ��Ȩλ
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
                zero  = true;//���㣬zero������ȷ�����������0ֻ��һ����
                chnStr.insert(0, chnNumChar[v]);
            }
        }
        else
        {
            zero = false;
            strIns = chnNumChar[v];//����
            strIns += chnUnitChar[unitPos]; //Ȩλ
            chnStr.insert(0, strIns);
        }
        unitPos ++;
        section = section / 10;
    }
}

