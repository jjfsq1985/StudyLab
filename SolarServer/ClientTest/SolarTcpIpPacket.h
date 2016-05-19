#pragma once
#include "pthread.h"
#include <vector>
#include <list>
using namespace std;

const byte SOLAR_PACKET_HEADER = 0x5A;  //��ͷ
const byte SOLAR_ESCAPE_CHAR = 0x5A;      //ת���ַ�1
const byte SOLAR_ESCAPE_ESCAPE = 0x5B;   //ת���ַ�2


typedef struct _tagSolarPacket
{    
    int nDstAddr;
    int nSrcAddr;
    byte byteCmd;
    int nDataLen;
    byte *m_Data;//������ת��
    byte xorData;
}SolarPacket;

class SolarTcpIpPacket
{
public:
    SolarTcpIpPacket();
    ~SolarTcpIpPacket();

public:
    static byte * MakeupPacket(const SolarPacket* cmdData, int& nRetLen);
    void DestroySolarPacket(SolarPacket* packet);

public:
    void StartParseThread();
    void StopParseThread();
    void AppendRecvData(int nLen, byte *pData);
    SolarPacket* PullRecvPacket();

public:
    static void* ParseRecvDataThread(void* lpParam);

private:
    vector<byte> PickPacket(vector<byte> vecData, int nStart, int nEnd, int& RemoveLen);
    void ParseRecvData();
    static byte CalcXOR(byte* pData, int nStart, int nEnd);
    byte CalcXOR(const vector<byte>& vecPacket);

private:
    bool m_threadRun;
    pthread_t m_threadId;
    pthread_mutex_t m_ParseMutex;
    pthread_cond_t m_ParseCond;

    int m_nRecvLen;
    vector<byte> m_vecRecvData;
    list<SolarPacket*> m_lstParsedPacket;
};

