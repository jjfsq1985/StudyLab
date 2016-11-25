#include "BigInt.h"


CBigInt::CBigInt()
{
	ZeroData();
}

CBigInt::CBigInt(const CBigInt& value)
{
	SetValue(value);
}

CBigInt::CBigInt(int64 value)
{
	SetValue(value);
}

CBigInt::~CBigInt()
{
}

int CBigInt::ToInt()
{
	int result = m_ulValue[0];
	if(m_sign != 0)
		result = -result;
	return result;
}

bool CBigInt::IsZero() const
{
	if( (m_nLength == 1) && (m_ulValue[0] == 0) )
		return true;
	return false;
}

//是否偶数
bool CBigInt::IsEven() const
{
	if( m_ulValue[0] & 0x00000001)
		return false;
	else
		return true;
}

CBigInt CBigInt::GetNegative() const
{
	CBigInt r = *this;
	r.m_sign = (this->m_sign == 0) ? 1 : 0;
	return r;
}

void CBigInt::ZeroData()
{
	m_sign = 0;
	m_nLength = 1;
	for(int i=0; i < MAX_BI_LEN; i++)
		m_ulValue[i] = 0;
}

const CBigInt& CBigInt::operator=(int64 value)
{
    SetValue(value);
    return *this;
}

const CBigInt& CBigInt::operator=(const CBigInt& value)
{
    SetValue(value);
    return *this;
}

void CBigInt::Add(const CBigInt& value1, const CBigInt& value2, CBigInt& result)
{
	result = value1;
	unsigned int carry = 0;//Add算法中只可能有0，1两种值
	if(result.m_nLength < value2.m_nLength)
		result.m_nLength = value2.m_nLength;
	for(unsigned int i= 0; i< result.m_nLength; i++)
	{
		uint64 sum = value2.m_ulValue[i];
		sum += result.m_ulValue[i] + carry;
		result.m_ulValue[i] = (unsigned long)(sum & 0xFFFFFFFF);
		carry = (unsigned int)((sum >> 32)&0xFFFFFFFF);
	}
	//处理最高位
	result.m_ulValue[result.m_nLength] = carry;
	result.m_nLength += carry;
}

void CBigInt::Sub(const CBigInt& value1, const CBigInt& value2, CBigInt& result)
{
    CBigInt r = value1;
    unsigned int borrow = 0;

    for (unsigned int i = 0; i < r.m_nLength; i++)
    {
        if ((r.m_ulValue[i] > value2.m_ulValue[i])
            || ((r.m_ulValue[i] == value2.m_ulValue[i]) && (borrow == 0)))
        {
            r.m_ulValue[i] = r.m_ulValue[i] - borrow - value2.m_ulValue[i];
            borrow = 0;
        }
        else
        {
            //0x100000000比INT_MAX大1
            uint64 num = 0x100000000 + r.m_ulValue[i];
            r.m_ulValue[i] = (unsigned long)(num - borrow - value2.m_ulValue[i]);
            borrow = 1;
        }
    }
    while ((r.m_ulValue[r.m_nLength - 1] == 0) && (r.m_nLength > 1) )
    {
        r.m_nLength--;
    }

    result = r;
}

CBigInt CBigInt::operator+(const CBigInt& value) const
{
	CBigInt r;
	if(m_sign == value.m_sign)
	{
		CBigInt::Add(*this, value, r);
		r.m_sign = m_sign;
	}
	else
	{
		if(CompareNoSign(value) >= 0)
		{
			CBigInt::Sub(*this, value, r);
            if(r.IsZero())
                r.m_sign = 0;
            else
                r.m_sign = m_sign;
		}
		else
		{
			CBigInt::Sub(value, *this, r);
			r.m_sign = value.m_sign;
		}
	}
	return r;
}

const CBigInt& CBigInt::operator+=(const CBigInt& value)
{
    *this = this->operator+(value);
    return *this;
}


CBigInt CBigInt::operator-(const CBigInt& value) const
{
    CBigInt r;
    if (m_sign != value.m_sign)
    {
        CBigInt::Add(*this, value, r);
        r.m_sign = m_sign;
    }
    else
    {
        if (CompareNoSign(value) >= 0)
        {
            CBigInt::Sub(*this, value, r);
            if(r.IsZero())
                r.m_sign = 0;
            else
                r.m_sign = m_sign;
        }
        else
        {
            CBigInt::Sub(value, *this, r);
            r.m_sign = (m_sign == 0 ? 1 : 0);//变号
        }
    }
    return r;
}

const CBigInt& CBigInt::operator-=(const CBigInt& value)
{
    *this = this->operator-(value);
    return *this;
}

CBigInt CBigInt::operator*(const CBigInt& value) const
{
    CBigInt r;
    if(m_sign != value.m_sign)
    {
        CBigInt::Mul(*this,value,r);
        r.m_sign = 1;
    }
    else
    {
        CBigInt::Mul(*this,value,r);
        r.m_sign = 0;
    }
    return r;
}

const CBigInt& CBigInt::operator*=(const CBigInt& value)
{
    *this = this->operator*(value);
    return *this;
}

CBigInt CBigInt::operator/(const CBigInt& value) const
{
    CBigInt r,m;
    if(m_sign != value.m_sign)
    {
        CBigInt::Div(*this, value, r,m);
        r.m_sign = 1;
    }
    else
    {
        CBigInt::Div(*this, value, r, m);
        r.m_sign = 0;
    }
    return r;
}

const CBigInt& CBigInt::operator/=(const CBigInt& value)
{
    *this = this->operator/(value);
    return *this;
}

CBigInt CBigInt::operator%(const CBigInt& value) const
{
    CBigInt r,m;
    CBigInt::Div(*this, value,r,m);
    m.m_sign = m_sign;
    return m;
}

const CBigInt& CBigInt::operator%=(const CBigInt& value)
{
    *this = this->operator%(value);
    return *this;
}

//不考虑符号位，比较数值大小
int CBigInt::CompareNoSign(const CBigInt& value) const
{
    if (m_nLength > value.m_nLength)
        return 1;
    if (m_nLength < value.m_nLength)
        return -1;
    for (int i = m_nLength - 1; i >= 0; i--)
    {
        if (m_ulValue[i] > value.m_ulValue[i])
            return 1;
        if (m_ulValue[i] < value.m_ulValue[i])
            return -1;
    }
    return 0;
}

void CBigInt::Mul(const CBigInt& value1, const CBigInt& value2, CBigInt& result)
{
    if(value1.IsZero() || value2.IsZero())
    {
        result = 0;
        return;
    }
    result = Karatsuba(value1, value2);
}

CBigInt CBigInt::MulVertical(const CBigInt& value1, const CBigInt& value2)
{
    CBigInt result;
    uint64 carry = 0;
    result.m_nLength = value1.m_nLength + value2.m_nLength - 1;//初步估算结果位数
    for (unsigned int i = 0; i < result.m_nLength; i++)
    {
        uint64 sum = carry;
        carry = 0;
        for (unsigned int j = 0; j < value2.m_nLength; j++)
        {
            if ((i - j >= 0) && ((i - j) < value1.m_nLength))
            {
                uint64 mul = value1.m_ulValue[i - j];
                mul *= value2.m_ulValue[j];
                carry += mul >> 32;
                mul = mul & 0xFFFFFFFF;
                sum += mul;
            }
        }
        carry += sum >> 32;
        result.m_ulValue[i] = sum & 0xFFFFFFFF;
    }
    if (carry != 0) //仍有进位,扩大位数
    {
        result.m_nLength++;
        result.m_ulValue[result.m_nLength - 1] = (unsigned long)(carry & 0xFFFFFFFF);
    }
    return result;
}

CBigInt CBigInt::Karatsuba(const CBigInt& value1, const CBigInt& value2)
{
    if (value1.m_nLength == 1 || value2.m_nLength == 1)
    {
        return MulVertical(value1, value2);
    }
    CBigInt high1, high2, low1, low2;
    unsigned int k = max(value1.m_nLength, value2.m_nLength) / 2;
    high1 = value1;
    //按k分割左右数据
    high1.GetRightBigN(k, low1);//low1 == 低k位数据
    high1.ShiftRightBigN(k); //high1 == 高位数据（value1.m_nLength - k）
    high2 = value2;
    high2.GetRightBigN(k, low2);
    high2.ShiftRightBigN(k);
    CBigInt z0 = Karatsuba(low1, low2);
    CBigInt z1 = Karatsuba((low1 + high1), (low2 + high2));
    CBigInt z2 = Karatsuba(high1, high2);
    CBigInt zk = z1 - z2 - z0;
    z2.ShiftLeftBigN(2 * k);
    zk.ShiftLeftBigN(k);

    return (z2 + zk + z0);
}

void CBigInt::GetRightBigN(unsigned int N, CBigInt& rightN)
{
    rightN = 0;
    unsigned int rN = min(m_nLength, N);
    for (unsigned int i = 0; i < rN; i++)
    {
        rightN.m_ulValue[i] = m_ulValue[i];
    }
    rightN.m_nLength = rN;
    rightN.m_sign = m_sign;
}

void CBigInt::ShiftLeftBigN(unsigned int N)
{
    if ((m_nLength + N) > MAX_BI_LEN)
        return;

    unsigned int i;
    for (i = m_nLength - 1; i >= 0; i--)
    {
        m_ulValue[N + i] = m_ulValue[i];
    }
    for (i = 0; i < N; i++)
    {
        m_ulValue[i] = 0;
    }
    m_nLength += N;
    return;
}

void CBigInt::ShiftRightBigN(unsigned int N)
{
    if (N >= m_nLength)
    {
        for(unsigned int i=0; i < m_nLength; i++)
        {
            m_ulValue[i] = 0;
        }
        return;
    }
    for (unsigned int i = 0; i < m_nLength; i++)
    {
        if (i < (m_nLength - N))
            m_ulValue[i] = m_ulValue[N + i];
        else
            m_ulValue[i] = 0;
    }

    m_nLength -= N;
}

void CBigInt::SetValue(const CBigInt& value)
{
    m_sign = value.m_sign;
    m_nLength = value.m_nLength;
    for(int i=0; i< MAX_BI_LEN; i++)
    {
        m_ulValue[i] = value.m_ulValue[i];
    }
}

void CBigInt::SetValue(int64 value)
{
    uint64 v;
    if(value >= 0)
    {
        m_sign = 0;
        v = value;
    }
    else
    {
        m_sign = 1;
        v = -value;
    }
    if(v > 0xFFFFFFFF)
    {
        m_nLength = 2;
        m_ulValue[1] = (unsigned long)(v >> 32);
        m_ulValue[0] = (unsigned long)(v & 0xFFFFFFFF);
    }
    else
    {
        m_nLength = 1;
        m_ulValue[0] = (unsigned long)v;
    }
    for(int i=m_nLength; i<MAX_BI_LEN; i++)
        m_ulValue[i] = 0;
}

bool CBigInt::operator<(const CBigInt& value) const
{
    if((m_sign == 0) && (value.m_sign == 1))
        return false;
    else if((m_sign == 1) && (value.m_sign == 0))
        return true;
    else if((m_sign == 0) && (value.m_sign == 0))
        return (CompareNoSign(value) < 0);
    else
        return (CompareNoSign(value) > 0);
}

bool CBigInt::operator<=(const CBigInt& value) const
{
    if((m_sign == 0) && (value.m_sign == 1))
        return false;
    else if((m_sign == 1) && (value.m_sign == 0))
        return true;
    else if((m_sign == 0) && (value.m_sign == 0))
        return (CompareNoSign(value) <= 0);
    else
        return (CompareNoSign(value) >= 0);
}

bool CBigInt::operator==(const CBigInt& value) const
{
    if(m_sign != value.m_sign)
        return false;
    else
        return (CompareNoSign(value) == 0);
}

bool CBigInt::operator!=(const CBigInt& value) const
{
    if((m_sign == value.m_sign) && (CompareNoSign(value) == 0))
        return false;
    else
        return true;
}

bool CBigInt::operator>(const CBigInt& value) const
{
    if((m_sign == 0) && (value.m_sign == 1))
        return true;
    else if((m_sign == 1) && (value.m_sign == 0))
        return false;
    else if((m_sign == 0) && (value.m_sign == 0))
        return (CompareNoSign(value) > 0);
    else
        return (CompareNoSign(value) < 0);
}

bool CBigInt::operator>=(const CBigInt& value) const
{
    if((m_sign == 0) && (value.m_sign == 1))
        return true;
    else if((m_sign == 1) && (value.m_sign == 0))
        return false;
    else if((m_sign == 0) && (value.m_sign == 0))
        return (CompareNoSign(value) >= 0);
    else
        return (CompareNoSign(value) <= 0);
}

CBigInt CBigInt::operator^(const CBigInt& n) const
{
    CBigInt r;
    CBigInt::Power(*this,n,r);
    if(n.IsEven())
    {
        r.m_sign = 0;
    }
    else
    {
        r.m_sign = this->m_sign;
    }

    return r;
}

const CBigInt& CBigInt::operator^=(const CBigInt& n)
{
    *this = this->operator^(n);
    return *this;
}

CBigInt CBigInt::operator<<(int64 bits) const
{
    CBigInt r;
    CBigInt::ShiftLeft(*this,bits,r);
    r.m_sign = this->m_sign;
    return r;
}

const CBigInt& CBigInt::operator<<=(int64 bits)
{
    CBigInt::ShiftLeft(*this,bits,*this);
    return *this;
}

CBigInt CBigInt::operator>>(int64 bits) const
{
    CBigInt r;
    CBigInt::ShiftRight(*this,bits, r);
    r.m_sign = this->m_sign;
    return r;
}

const CBigInt& CBigInt::operator>>=(int64 bits)
{
    CBigInt::ShiftRight(*this,bits,*this);
    return *this;
}

void CBigInt::Div(const CBigInt& value1,const CBigInt& value2,CBigInt& quotient, CBigInt& remainder)
{
    CBigInt r = 0;
    CBigInt a = value1;
    while(a.CompareNoSign(value2) >= 0)
    {
        uint64 div = a.m_ulValue[a.m_nLength - 1];
        uint64 num = value2.m_ulValue[value2.m_nLength - 1];
        unsigned int len = a.m_nLength - value2.m_nLength;
        if((div == num) && (len == 0))
        {
            CBigInt::Add(r,CBigInt(1),r);
            CBigInt::Sub(a,value2,a);
            break;
        }
        if((div <= num) && (len > 0))
        {
            len--;
            div = (div << 32) + a.m_ulValue[a.m_nLength -2];
        }
        div = div / (num + 1);
        CBigInt multi = div; //试商的结果
        if(len > 0)
        {
            multi.m_nLength += len;
            unsigned int i;
            for(i = multi.m_nLength -1; i>= len; i--)
            {
                multi.m_ulValue[i] = multi.m_ulValue[i-len];
            }
            for(i=0; i< len; i++)
            {
                multi.m_ulValue[i] = 0;
            }
        }
        CBigInt tmp;
        CBigInt::Add(r,multi,r);
        CBigInt::Mul(value2,multi,tmp);
        CBigInt::Sub(a,tmp,a);
    }
    quotient = r;
    remainder = a;
}

void CBigInt::Mod(const CBigInt &value1, const CBigInt &value2, CBigInt &result)
{
    CBigInt a = value1;
    while(a.CompareNoSign(value2) >= 0)
    {
        uint64 div = a.m_ulValue[a.m_nLength - 1];
        uint64 num = value2.m_ulValue[value2.m_nLength - 1];
        unsigned int len = a.m_nLength - value2.m_nLength;
        if((div == num) && (len == 0))
        {
            CBigInt::Sub(a,value2,a);
            break;
        }
        if((div <= num) && (len > 0))
        {
            len--;
            div = (div << 32) + a.m_ulValue[a.m_nLength -2];
        }
        div = div / (num + 1);
        CBigInt multi = div; //试商的结果
        CBigInt::Mul(value2,multi,multi);
        if(len > 0)
        {
            multi.m_nLength += len;
            unsigned int i;
            for(i = multi.m_nLength -1; i>= len; i--)
            {
                multi.m_ulValue[i] = multi.m_ulValue[i-len];
            }
            for(i=0; i< len; i++)
            {
                multi.m_ulValue[i] = 0;
            }
        }
        CBigInt::Sub(a,multi,a);
    }
    result = a;
}

void CBigInt::Power(const CBigInt& value, const CBigInt& n, CBigInt& result)
{
    result = 1;
    CBigInt t = value;
    for(int64 i=0; i< n.GetTotalBits(); i++)
    {
        if(n.TestBit(i))
        {
            CBigInt::Mul(result,t,result);
        }
        CBigInt::Mul(t,t,t);
    }
}
bool CBigInt::TestBit(int64 bits) const
{
    int64 wp = bits / 32;
    int64 bp = bits % 32;
    if(wp >= m_nLength)
        return false;
    int64 bitMask = 0x1 << bp;
    return (m_ulValue[wp] & bitMask) != 0;

}

int64 CBigInt::GetTotalBits() const
{
    int64 cnt = (m_nLength -1 ) * 32;
    unsigned long mv = m_ulValue[m_nLength - 1];
    while(mv > 0)
    {
        mv = mv >> 1;
        cnt++;
    }
    return cnt;

}

void CBigInt::ShiftLeft(const CBigInt& value, int64 bits, CBigInt& result)
{
    int64 wPos = bits / 32;
    int64 wOff = bits % 32;

    result = value;
    if(wOff != 0)
    {
        unsigned long maskRight = 0xFFFFFFFF >> wOff;
        unsigned long maskLeft = ~maskRight;
        result.m_ulValue[result.m_nLength] = 0;
        result.m_nLength++;
        for(int i = result.m_nLength - 1; i >=0; i--)
        {
            unsigned long orp = result.m_ulValue[i] & maskRight;
            if(i > 0)
            {
                unsigned long nlp = result.m_ulValue[i - 1] & maskLeft;
                result.m_ulValue[i] = (orp << wOff) | (nlp >> (32 - wOff));
            }
            else
            {
                result.m_ulValue[i] = orp << wOff;
            }
        }
        if(result.m_ulValue[result.m_nLength - 1] == 0)
            result.m_nLength--;
    }
    if(wPos != 0)
    {
        for(int j = result.m_nLength - 1; j >= 0; j--)
        {
            result.m_ulValue[j + wPos] = result.m_ulValue[j];
        }
        for(int k = 0; k < wPos; k++)
        {
            result.m_ulValue[k] = 0;
        }
        result.m_nLength += (unsigned int)wPos;
    }
}

void CBigInt::ShiftRight(const CBigInt& value, int64 bits, CBigInt& result)
{
    if(bits >= value.GetTotalBits())
    {
        result = 0;
        return;
    }

    int64 wPos = bits / 32;
    int64 wOff = bits % 32;

    result = value;
    if(wOff != 0)
    {
        unsigned long maskLeft = 0xFFFFFFFF << wOff;
        unsigned long maskRight = ~maskLeft;
        for(unsigned int i = 0; i < result.m_nLength; i++)
        {
            unsigned long olp = result.m_ulValue[i] & maskLeft;
            if(i < (result.m_nLength - 1))
            {
                unsigned long nrp = result.m_ulValue[i + 1] & maskRight;
                result.m_ulValue[i] = (olp >> wOff) | (nrp << (32 - wOff));
            }
            else
            {
                result.m_ulValue[i] = olp >> wOff;
            }
        }
        if(result.m_ulValue[result.m_nLength - 1] == 0)
            result.m_nLength--;
    }
    if(wPos != 0)
    {
        if(result.m_nLength <= wPos)
        {
            result = 0;
        }
        else
        {
            unsigned int j;
            for(j = 0; j < (result.m_nLength - wPos); j++)
            {
                result.m_ulValue[j] = result.m_ulValue[j + wPos];
            }
            while(j < result.m_nLength)
            {
                result.m_ulValue[j] = 0;
                j++;
            }

            result.m_nLength -= (unsigned int)wPos;
        }
    }
}


CBigInt CBigInt::GenRandomInteger(int64 bits)
{
    CBigInt r;

    r.m_sign = 0;
    r.m_nLength = (unsigned int)(bits / 32);

    for(unsigned int i = 0; i < r.m_nLength; i++)
        r.m_ulValue[i] = rand()*0x10000 + rand();

    int64 wOff = bits % 32;
    if(wOff != 0)
    {
        r.m_nLength++;
        r.m_ulValue[r.m_nLength - 1] = rand()*0x10000 + rand();
        r.m_ulValue[r.m_nLength - 1] = r.m_ulValue[r.m_nLength - 1] & (0xFFFFFFFF >> (32 - wOff));
    }

    return r;
}

void CBigInt::GetFromString(const char *str, unsigned int frombase)
{
    ZeroData();
    int i = 0;
    int len = (int)strlen(str);
    int k;
    if((len == 0) || ((len == 1) && (str[0] == '-')))
    {
        return;
    }
    if(str[0] == '-')
        i++;

    while(i < len)
    {
       CBigInt::Mul(*this, frombase, *this);
       if((str[i]>='0')&&(str[i]<='9'))
           k = str[i] - 48;
       else if((str[i]>='A')&&(str[i]<='F'))
           k = str[i] - 55;
       else if((str[i]>='a')&&(str[i]<='f'))
           k = str[i] - 87;
       else
           k = 0;
       CBigInt::Add(*this, k, *this);
       i++;
    }
    if(str[0] == '-')
        m_sign = 1;
}

void CBigInt::PutToString(string& str, unsigned int frombase)
{
    if((m_nLength == 1) && (m_ulValue[0] == 0))
    {
        str="0";
        return;
    }
	str="";
    const char *t="0123456789ABCDEF";
    CBigInt value = *this;
    CBigInt base = frombase;
    while(value.m_ulValue[value.m_nLength - 1] > 0)
    {
        CBigInt a;
        CBigInt::Div(value, base, value, a);
        str.insert(str.begin(), t[abs(a.ToInt())]);
    }
    if(m_sign == 1)
    {
        str.insert(str.begin(), '-');
    }
}

//输入到CBigInt中
void CBigInt::GetLong(unsigned long *values, unsigned int count)
{
    ZeroData();
    m_nLength = count;
    for(unsigned int i = 0; i < count; i++)
        m_ulValue[i] = values[i];
}

//从CBigInt输出到long数组中
int CBigInt::PutLong(unsigned long *values, unsigned int bufLen)
{
    if(m_nLength > bufLen)
        return 0;

    for(unsigned int i = 0; i < m_nLength; i++)
        values[i] = m_ulValue[i];

    return m_nLength;
}

