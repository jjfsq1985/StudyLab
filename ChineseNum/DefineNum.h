#ifndef DEFINE_NUM_H
#define DEFINE_NUM_H
#include <string>

const int CHN_CHAR_LENGTH = 2;  //每个数字的中文占2个字节

const int CHN_NUM_CHAR_COUNT = 10;
const std::string chnNumChar[CHN_NUM_CHAR_COUNT] = { "零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖" };
const std::string chnNumChar_s[CHN_NUM_CHAR_COUNT] = { "零", "一", "二", "三", "四", "五", "六", "七", "八", "九" };

const int CHN_UNIT_SECTION_COUNT = 5;
const std::string chnUnitSection[CHN_UNIT_SECTION_COUNT] = { "", "万", "亿", "万亿", "亿亿" };

const int CHN_UNIT_CHAR_COUNT = 4;
const std::string chnUnitChar[CHN_UNIT_CHAR_COUNT] = { "", "拾", "佰", "仟" };

#endif // DEFINE_NUM_H
