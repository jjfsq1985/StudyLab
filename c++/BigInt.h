#ifndef _BIGINT_
#define _BIGINT_

#include <string.h>
#include <algorithm>
#include <string>
using namespace std;

const int MAX_BI_LEN = 128;
typedef long long int64;
typedef unsigned long long uint64;

class CBigInt
{
public:
	CBigInt();
	CBigInt(const CBigInt& value);
	CBigInt(int64 value);
	~CBigInt();
private:
	unsigned int m_sign;//符号位 0 正数 1 负数
	unsigned int m_nLength;    //m_ulValue数据个数
	unsigned long m_ulValue[MAX_BI_LEN];//数组记录的大数在2^32进制下的值

public:
    CBigInt GenRandomInteger(int64 bits);
    void GetFromString(const char *str, unsigned int frombase);
    void PutToString(std::string& str, unsigned int frombase);
    void GetLong(unsigned long *values, unsigned int count);
    int PutLong(unsigned long *values, unsigned int bufLen);
    int ToInt();
    bool IsZero() const;
    bool IsEven() const;
    CBigInt GetNegative() const;
    bool TestBit(int64 bits) const;
    int64 GetTotalBits() const;

public:
    const CBigInt& operator=(int64 value);
    const CBigInt& operator=(const CBigInt& value);
    bool operator<(const CBigInt& value) const;
    bool operator<=(const CBigInt& value) const;
    bool operator==(const CBigInt& value) const;
    bool operator!=(const CBigInt& value) const;
    bool operator>(const CBigInt& value) const;
    bool operator>=(const CBigInt& value) const;


protected:
    void ZeroData();
    void SetValue(const CBigInt& value);
    void SetValue(int64 value);
    int CompareNoSign(const CBigInt& value) const;
    static void Add(const CBigInt& value1, const CBigInt& value2, CBigInt& result);
    static void Sub(const CBigInt& value1, const CBigInt& value2, CBigInt& result);
    static void Mul(const CBigInt& value1, const CBigInt& value2, CBigInt& result);
    static void Div(const CBigInt& value1, const CBigInt& value2, CBigInt& quotient, CBigInt& remainder);
    static void Mod(const CBigInt& value1, const CBigInt& value2, CBigInt& result);
    static void Power(const CBigInt& value, const CBigInt& n, CBigInt& result);

    //左移
    static void ShiftLeft(const CBigInt& value, int64 bits, CBigInt& result);
    //右移
    static void ShiftRight(const CBigInt& value, int64 bits, CBigInt& result);

private:

    void GetRightBigN(unsigned int N, CBigInt& rightN);
    void ShiftLeftBigN(unsigned int N);
    void ShiftRightBigN(unsigned int N);
    static CBigInt MulVertical(const CBigInt& value1, const CBigInt& value2);
    static CBigInt Karatsuba(const CBigInt& value1, const CBigInt& value2);

public:
    CBigInt operator+(const CBigInt& value) const;
    const CBigInt& operator+=(const CBigInt& value);
    CBigInt operator-(const CBigInt& value) const;
    const CBigInt& operator-=(const CBigInt& value);
    CBigInt operator*(const CBigInt& value) const;
    const CBigInt& operator*=(const CBigInt& value);
    CBigInt operator/(const CBigInt& value) const;
    const CBigInt& operator/=(const CBigInt& value);
    CBigInt operator%(const CBigInt& value) const;
    const CBigInt& operator%=(const CBigInt& value);
    CBigInt operator^(const CBigInt& n) const;
    const CBigInt& operator^=(const CBigInt& n);
    CBigInt operator<<(int64 bits) const;
    const CBigInt& operator<<=(int64 bits);
    CBigInt operator>>(int64 bits) const;
    const CBigInt& operator>>=(int64 bits);
};

#endif
