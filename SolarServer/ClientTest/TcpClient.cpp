#include "stdafx.h"
#include "TcpClient.h"
#include "SolarTcpIpPacket.h"

#include<ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include "event2\bufferevent.h"
#include "event2\buffer.h"
#include "event2\listener.h"
#include "event2\util.h"
#include "event2\event.h"
#include "event2\thread.h"

struct bufferevent * TcpClient::m_pBev = NULL;

TcpClient::TcpClient()
    :m_bConnecting(false)
    , m_bConnected(false)
    , m_nPort(0)
{
    m_pPacket = new SolarTcpIpPacket();
    m_pEmulation = new ClientEmulation(this);

    strcpy_s(m_cIPAddr, 16,"127.0.0.1");
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}


TcpClient::~TcpClient()
{
    m_bConnecting = false;
    m_pPacket->StopParseThread();
    delete m_pPacket;
    m_pEmulation->StopWork();
    delete m_pEmulation;
    m_bConnected = false;
    WSACleanup();
}


bool TcpClient::Init(const char *cIPAddr, int nPort)
{
    m_pPacket->StartParseThread();
    m_pEmulation->StartWork();
    strcpy_s(m_cIPAddr, 16, cIPAddr);
    m_nPort = nPort;
    m_bConnecting = true;
    pthread_t tid;
    pthread_create(&tid, NULL, ConnectThread, this);
    return true;
}

//调用bufferevent_write后会触发该事件，如果bufferevent_setcb中设置了bufferevent_data_cb的话
void TcpClient::write_cb(struct bufferevent *bev, void *arg)
{
    //do nothing
}

void* TcpClient::ConnectThread(void *pParam)
{
    TcpClient *pClient = (TcpClient *)pParam;

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    inet_pton(AF_INET, pClient->m_cIPAddr, (void*)&sin.sin_addr);
    sin.sin_port = htons(pClient->m_nPort);

    int nCycle = 15;
    while (pClient->m_bConnecting)
    {
        if (nCycle < 15)
        {
            nCycle++;
            Sleep(1000);
            continue;
        }
        Tprintf(L"Connecting\n");
        nCycle = 0;
        struct event_base *base = event_base_new();
        assert(base != NULL);
#if defined (WIN32)
        evthread_use_windows_threads();
#else
        evthread_use_pthreads();
#endif
        evthread_make_base_notifiable(base);
        struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
        bufferevent_setcb(bev, read_cb, NULL, event_cb, pClient);
        bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST);
        //连接
        if (bufferevent_socket_connect(bev, (SOCKADDR*)&sin, sizeof(SOCKADDR)) < 0)
        {
            bufferevent_free(bev);
            return NULL;
        }
        event_base_dispatch(base);

        event_base_free(base);
    }
    return NULL;
}

//从socket读
void TcpClient::read_cb(struct bufferevent *bev, void *arg)
{
    TcpClient *pClient = (TcpClient *)arg;
    const int MAX_LENGHT = 1024;
    char cbData[MAX_LENGHT];
    int n;
    //读数据：bufferevent_read
    //写数据：bufferevent_write
    while (n = bufferevent_read(bev, cbData, MAX_LENGHT))
    {
        if (n <= 0)
            break;
        //处理接收到的数据
        pClient->DealWithData(bev,cbData, n);
    }
}

void TcpClient::event_cb(struct bufferevent *bev, short event, void *arg)
{
    TcpClient *pClient = (TcpClient *)arg;
    evutil_socket_t fd = bufferevent_getfd(bev);
    Tprintf(L"fd = %u, ", fd);
    if (event & BEV_EVENT_TIMEOUT)
    {
        Tprintf(L"Timed out\n"); //if bufferevent_set_timeouts() called
    }
    else if (event & BEV_EVENT_CONNECTED)
    {
        pClient->m_pBev = bev;
        pClient->m_bConnected = true;
        Tprintf(L"Connect okay.\n");
    }
    else if (event & BEV_EVENT_EOF)
    {
        pClient->m_bConnected = false;
        Tprintf(L"connection closed\n");
        bufferevent_free(bev);
    }
    else if (event & BEV_EVENT_ERROR)
    {
        pClient->m_bConnected = false;
        Tprintf(L"some other error\n");
        bufferevent_free(bev);
    }
}

bool TcpClient::SendData(const char *pData, int nLen)
{
    if (!m_bConnected)
        return false;
    if (bufferevent_write(m_pBev, pData, nLen) < 0)
        return false;
    else
        return true;
}

//解析从Server发出的数据
bool TcpClient::DealWithData(struct bufferevent *bev, const char*pData, int nLen)
{
    evutil_socket_t fd = bufferevent_getfd(bev);
    struct sockaddr_in sin;
    int nsocketLen = sizeof(SOCKADDR);
    char cIPParse[16];
    ushort port;
    if (getpeername(fd, (struct sockaddr*)&sin, &nsocketLen) == 0)
    {
        inet_ntop(AF_INET, (void *)&sin.sin_addr, cIPParse, 16);
        port = ntohs(sin.sin_port);
    }

    m_pPacket->AppendRecvData(nLen, (byte*)pData);
    SolarPacket* pPacketData = m_pPacket->PullRecvPacket();
    if (pPacketData != NULL)
    {
        switch (pPacketData->byteCmd)
        {
        case 0x10:
            break;
        case 0x11:
            break;
        case 0x12:
            break;
        case 0x13:
            break;
        case 0x14:
            break;
        case 0x15:
            break;
        }

        SolarTcpIpPacket::DestroySolarPacket(pPacketData);
    }
    return false;
}
