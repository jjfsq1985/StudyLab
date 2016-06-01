#include "stdafx.h"
#include "ClientEmulation.h"
#include "SolarTcpIpPacket.h"
#include "TcpClient.h"


ClientEmulation::ClientEmulation(class TcpClient *pClient)
    :m_pClient(pClient)
{
    m_threadRun = false;
    m_lstccdpar.clear();
    pthread_mutex_init(&m_mutex, NULL);
}


ClientEmulation::~ClientEmulation()
{
    pthread_mutex_destroy(&m_mutex);
}

void ClientEmulation::StartWork()
{
    m_threadRun = true;
    pthread_create(&m_threadId, NULL, CCDdataEmulation, this);
    pthread_create(&m_SendId, NULL, SendSvr, this);
}

void* ClientEmulation::SendSvr(void* pParam)
{
    ClientEmulation *pCtrl = (ClientEmulation*)pParam;
    while (pCtrl->m_threadRun)
    {
        pCtrl->SendServerProc();
        Sleep(100);
    }
    return NULL;
}

void ClientEmulation::SendServerProc()
{
    if (!m_pClient->m_bConnected)
        return;
    CCDParam param = GetData();
    if (param.nValid != 1)
        return;
    SolarPacket *pPacket = new SolarPacket();
    pPacket->nDstAddr = 4;
    pPacket->nSrcAddr = 1;
    pPacket->byteCmd = 0x13;
    pPacket->nDataLen = sizeof(CCDParam);
    pPacket->m_Data = new byte[sizeof(CCDParam)];
    memcpy(pPacket->m_Data, &param, sizeof(CCDParam));
    int nLen = 0;
    byte * pSend = SolarTcpIpPacket::MakeupPacket(pPacket, nLen);

    m_pClient->SendData((char*)pSend, nLen);
    delete[] pSend;

    SolarTcpIpPacket::DestroySolarPacket(pPacket);
}

void ClientEmulation::StopWork()
{
    m_threadRun = false;
    pthread_join(m_threadId, NULL);
    pthread_join(m_SendId, NULL);    
}

void* ClientEmulation::CCDdataEmulation(void * pParam)
{
    ClientEmulation *pCtrl = (ClientEmulation*)pParam;
    pCtrl->CCDEmulation();
    return NULL;
}

void ClientEmulation::CCDEmulation()
{
    CCDParam param;
    while (m_threadRun)
    {
        pthread_mutex_lock(&m_mutex);
        param.nSawMarkGroove = rand() % 3;
        param.SawMarkStep = rand() % 3;
        param.SawMarkEdge = rand() % 3;
        param.sori = rand() % 10;
        param.bow = rand() % 10;
        param.chips = rand() % 3;
        param.breakages = rand() % 3;
        param.holes = 0;
        param.cracks = 0;
        param.NVCD = 0;
        param.stain = 0;
        param.glue = 0;
        param.nValid = 1;
        m_lstccdpar.push_back(param);
        pthread_mutex_unlock(&m_mutex);
        Sleep(100);
    }
}

CCDParam ClientEmulation::GetData()
{
    if (m_lstccdpar.size() <= 0)
    {
        CCDParam data;
        data.nValid = 0;
        return data;
    }
    pthread_mutex_lock(&m_mutex);

    CCDParam data = m_lstccdpar.front();
    m_lstccdpar.pop_front();
    pthread_mutex_unlock(&m_mutex);
    return data;

}