#include "stdafx.h"
#include "SolarTcpIpPacket.h"


SolarTcpIpPacket::SolarTcpIpPacket()
{
    pthread_mutex_init(&m_ParseMutex, NULL);
    pthread_cond_init(&m_ParseCond, NULL);
    m_nRecvLen = 0;
    m_threadRun = false;
    m_vecRecvData.clear();
    m_lstParsedPacket.clear();    
}


SolarTcpIpPacket::~SolarTcpIpPacket()
{
    for (list<SolarPacket*>::iterator it = m_lstParsedPacket.begin(); it != m_lstParsedPacket.end(); it++)
    {
        delete[] (*it)->m_Data;
        delete (*it);
    }
    m_lstParsedPacket.clear();
    pthread_mutex_destroy(&m_ParseMutex);
    pthread_cond_destroy(&m_ParseCond);
}


void SolarTcpIpPacket::StartParseThread()
{
    m_threadRun = true;
    pthread_create(&m_threadId, NULL, ParseRecvDataThread, this);
}

void SolarTcpIpPacket::StopParseThread()
{
    m_threadRun = false;
    pthread_cond_signal(&m_ParseCond);
    pthread_join(m_threadId, NULL);
}

void SolarTcpIpPacket::AppendRecvData(int nLen, byte *pData)
{
    if (nLen <= 0)
        return;
    pthread_mutex_lock(&m_ParseMutex);
    m_vecRecvData.resize(m_nRecvLen + nLen);
    std::copy(pData, pData + nLen, m_vecRecvData.begin() + m_nRecvLen);
    m_nRecvLen += nLen;
    pthread_cond_signal(&m_ParseCond);
    pthread_mutex_unlock(&m_ParseMutex);
}

void* SolarTcpIpPacket::ParseRecvDataThread(void* lpParam)
{
    SolarTcpIpPacket *pCallPtr = (SolarTcpIpPacket*)lpParam;
    pCallPtr->ParseRecvData();    
    return NULL;
}

vector<byte> SolarTcpIpPacket::PickPacket(vector<byte> vecData, int nStart, int nEnd, int& RemoveLen)
{
    RemoveLen = 0;
    if (nEnd <= nStart)
        return vector<byte>();
    bool bRet = false;
    vector<byte> vecPacket;
    int nPushLen = 0;
    int nCmdDataLen = 0;
    for (int i = nStart; i < nEnd; i++)
    {
        if (vecData[i] == SOLAR_PACKET_HEADER)
        {
            vecPacket.push_back(vecData[i]);
            nPushLen++;
        }
        else if (vecData[i] == SOLAR_ESCAPE_ESCAPE)
        {
            if (vecData[i + 1] == 0x3A)
            {
                vecPacket.push_back(0x5A);
                nPushLen++;
            }
            else if (vecData[i + 1] == 0x3B)
            {
                vecPacket.push_back(0x5B);
                nPushLen++;
            }
            else
            {
                Tprintf(L"Packet Error\n");
                break;
            }
            i++;
        }
        else
        {
            vecPacket.push_back(vecData[i]);
            nPushLen++;
        }
        if (nPushLen > 8)
        {
            nCmdDataLen = (vecPacket[6]<<8) + vecPacket[7];
            if (nPushLen == nCmdDataLen + 9)
            {
                bRet = true;
                RemoveLen = i+1;
                break;
            }
        }
    }
    return bRet ? vecPacket : vector<byte>();
}

void SolarTcpIpPacket::ParseRecvData()
{
    while (m_threadRun)
    {
        pthread_mutex_lock(&m_ParseMutex);
        pthread_cond_wait(&m_ParseCond, &m_ParseMutex);
        if (!m_threadRun)
        {
            pthread_mutex_unlock(&m_ParseMutex);
            break;
        }
        if (m_nRecvLen > 9)
        {
            //Ω‚ŒˆData
            int nPos = 0;
            int nEndPos = 0;
            while (nPos < m_nRecvLen)
            {
                if (nPos + 9 >= m_nRecvLen)
                    break;
                if (m_vecRecvData[nPos] == SOLAR_PACKET_HEADER)
                {
                    int nRemoveLen = 0;
                    vector<byte> vecPacket = PickPacket(m_vecRecvData, nPos, m_nRecvLen, nRemoveLen);
                    if (nRemoveLen > 0)
                    {
                        m_vecRecvData.erase(m_vecRecvData.begin(), m_vecRecvData.begin() + nRemoveLen);
                        int nLen = vecPacket.size();
                        SolarPacket* packetSingle = new SolarPacket();
                        packetSingle->nDstAddr = (vecPacket[1] << 8) + vecPacket[2];
                        packetSingle->nSrcAddr = (vecPacket[3] << 8) + vecPacket[4];
                        packetSingle->byteCmd = vecPacket[5];
                        packetSingle->nDataLen = (vecPacket[6] << 8) + vecPacket[7];
                        assert(packetSingle->nDataLen > 0);
                        packetSingle->m_Data = new byte[packetSingle->nDataLen];
                        for (int i = 0; i < packetSingle->nDataLen; i++)
                            packetSingle->m_Data[i] = vecPacket[8 + i];
                        packetSingle->xorData = vecPacket[nLen - 1];
                        assert(CalcXOR(vecPacket) == packetSingle->xorData);
                        m_lstParsedPacket.push_back(packetSingle);
                        nPos += nRemoveLen;
                    }
                }                
            }
        }
        pthread_mutex_unlock(&m_ParseMutex);
    }
}

byte SolarTcpIpPacket::CalcXOR(const vector<byte>& vecPacket)
{
    int nLen = vecPacket.size();
    if (nLen < 9)
        return 0;
    byte xorVal = 0;
    for (int i = 5; i < nLen-1; i++)
        xorVal ^= vecPacket[i];
    return xorVal;
}


byte SolarTcpIpPacket::CalcXOR(byte* pData, int nStart, int nEnd)
{
    byte xorVal = 0;
    for (int i = nStart; i < nEnd; i++)
        xorVal ^= pData[i];
    return xorVal;
}

void SolarTcpIpPacket::DestroySolarPacket(SolarPacket* packet)
{
    if (packet == NULL)
        return;
    if (packet->nDataLen > 0 && packet->m_Data != NULL)
        delete[] packet->m_Data;
    delete packet;
}

SolarPacket* SolarTcpIpPacket::PullRecvPacket()
{
    if (m_lstParsedPacket.size() <= 0)
        return NULL;
    pthread_mutex_lock(&m_ParseMutex);
    SolarPacket* data = m_lstParsedPacket.front();
    m_lstParsedPacket.pop_front();
    pthread_mutex_unlock(&m_ParseMutex);
    return data;
}

byte * SolarTcpIpPacket::MakeupPacket(const SolarPacket* cmdData, int& nRetLen)
{
    nRetLen = 0;
    byte*pSrcData = new byte[cmdData->nDataLen + 9];
    pSrcData[0] = 0x5A;
    pSrcData[1] = (cmdData->nDstAddr >> 8) & 0xFF;
    pSrcData[2] = cmdData->nDstAddr & 0xFF;
    pSrcData[3] = (cmdData->nSrcAddr >> 8) & 0xFF;
    pSrcData[4] = cmdData->nSrcAddr & 0xFF;
    pSrcData[5] = cmdData->byteCmd;
    pSrcData[6] = (cmdData->nDataLen >> 8) & 0xFF;
    pSrcData[7] = cmdData->nDataLen & 0xFF;
    memcpy(pSrcData + 8, cmdData->m_Data, cmdData->nDataLen*sizeof(byte));
    int nPackLen = 9 + cmdData->nDataLen;
    pSrcData[8 + cmdData->nDataLen] = CalcXOR(pSrcData, 5,nPackLen-1);
    int nRepeat = 0;
    for (int i = 1; i < nPackLen; i++)
    {
        if (pSrcData[i] == SOLAR_ESCAPE_CHAR || pSrcData[i] == SOLAR_ESCAPE_ESCAPE)
            nRepeat++;
    }
    nRetLen = cmdData->nDataLen + 9 + nRepeat;
    byte*pRetData = new byte[nRetLen];
    pRetData[0] = SOLAR_PACKET_HEADER;
    int nPos = 1;
    for (int n = 1; n < nPackLen; n++)//HeaderÃ¯π˝
    {
        if (pSrcData[n] == SOLAR_ESCAPE_CHAR)
        {
            pRetData[nPos] = 0x5B;
            pRetData[nPos + 1] = 0x3A;
            nPos += 2;
        }
        else if (pSrcData[n] == SOLAR_ESCAPE_ESCAPE)
        {
            pRetData[nPos] = 0x5B;
            pRetData[nPos + 1] = 0x3B;
            nPos += 2;
        }
        else
        {
            pRetData[nPos] = pSrcData[n];
            nPos++;
        }
    }
    delete[] pSrcData;

    return pRetData;
}