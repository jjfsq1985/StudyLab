#ifndef DEFINE_NUM_H
#define DEFINE_NUM_H
#include <string>

const int CHN_CHAR_LENGTH = 2;  //ÿ�����ֵ�����ռ2���ֽ�

const int CHN_NUM_CHAR_COUNT = 10;
const std::string chnNumChar[CHN_NUM_CHAR_COUNT] = { "��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��" };
const std::string chnNumChar_s[CHN_NUM_CHAR_COUNT] = { "��", "һ", "��", "��", "��", "��", "��", "��", "��", "��" };

const int CHN_UNIT_SECTION_COUNT = 5;
const std::string chnUnitSection[CHN_UNIT_SECTION_COUNT] = { "", "��", "��", "����", "����" };

const int CHN_UNIT_CHAR_COUNT = 4;
const std::string chnUnitChar[CHN_UNIT_CHAR_COUNT] = { "", "ʰ", "��", "Ǫ" };

#endif // DEFINE_NUM_H
