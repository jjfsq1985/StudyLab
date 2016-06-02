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
    for (list<CCDParam*>::iterator it = m_lstccdpar.begin(); it != m_lstccdpar.end(); it++)
    {
        delete (*it);
    }
    m_lstccdpar.clear();
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
    CCDParam* pParam = PopCCDdata();
    if (pParam == NULL)
        return;
    SolarPacket *pPacket = new SolarPacket();
    pPacket->nDstAddr = 4;
    pPacket->nSrcAddr = 1;
    pPacket->byteCmd = 0x13;
    pPacket->nDataLen = sizeof(CCDParam);
    pPacket->m_Data = new byte[sizeof(CCDParam)];
    memcpy(pPacket->m_Data, pParam, sizeof(CCDParam));
    int nLen = 0;
    byte * pSend = SolarTcpIpPacket::MakeupPacket(pPacket, nLen);
    m_pClient->SendData((char*)pSend, nLen);
    delete pParam;
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
    CCDParam *pParam = NULL;
    while (m_threadRun)
    {
        pthread_mutex_lock(&m_mutex);
        pParam = new CCDParam;
        pParam->nSawMarkGroove = rand() % 3;
        pParam->SawMarkStep = rand() % 3;
        pParam->SawMarkEdge = rand() % 3;
        pParam->sori = rand() % 10;
        pParam->bow = rand() % 10;
        pParam->chips = rand() % 3;
        pParam->breakages = rand() % 3;
        pParam->holes = 0;
        pParam->cracks = 0;
        pParam->NVCD = 0;
        pParam->stain = 0;
        pParam->glue = 0;
        m_lstccdpar.push_back(pParam);
        pthread_mutex_unlock(&m_mutex);
        Sleep(100);
    }
}

CCDParam* ClientEmulation::PopCCDdata()
{
    if (m_lstccdpar.size() <= 0)
    {
        return NULL;
    }
    pthread_mutex_lock(&m_mutex);

    CCDParam* pCCDdata = m_lstccdpar.front();
    m_lstccdpar.pop_front();
    pthread_mutex_unlock(&m_mutex);
    return pCCDdata;

}