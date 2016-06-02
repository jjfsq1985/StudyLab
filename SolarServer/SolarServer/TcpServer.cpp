#include "stdafx.h"
#include "TcpServer.h"
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

#define LISTEN_BACKLOG 32

typedef struct _cbListenParam
{
    class TcpServer *pSvr;
    struct event_base *ListenBase;
}ListenParam;

vector<struct bufferevent*> TcpServer::m_VecBev;

TcpServer::TcpServer()
{
    m_mapPacket.clear();
    WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
}


TcpServer::~TcpServer()
{
    for (map<evutil_socket_t, class SolarTcpIpPacket *>::iterator it = m_mapPacket.begin(); it != m_mapPacket.end(); it++)
    {
        it->second->StopParseThread();
        delete it->second;
    }
    WSACleanup();
}


bool TcpServer::Init(int nPort)
{
    m_nListenPort = nPort;
    pthread_t tId;
    pthread_create(&tId, NULL, ListenThread, this);
    return true;
}

void* TcpServer::ListenThread(void *pParam)
{
    TcpServer *pSvr = (TcpServer *)pParam;

    evutil_socket_t listener;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener > 0);
    evutil_make_listen_socket_reuseable(listener);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(pSvr->m_nListenPort);

    if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return NULL;
    }

    if (listen(listener, LISTEN_BACKLOG) < 0) {
        perror("listen");
        return NULL;
    }

    Tprintf(L"Server Port %d Listening...\n",pSvr->m_nListenPort);
    

    evutil_make_socket_nonblocking(listener);

    struct event_base *base = event_base_new();
    assert(base != NULL);
    struct event *listen_event;
    ListenParam cbListen;
    cbListen.pSvr = pSvr;
    cbListen.ListenBase = base;
    listen_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)&cbListen);
    event_add(listen_event, NULL);
    event_base_dispatch(base);
    event_base_free(base);
    evutil_closesocket(listener);
    return NULL;
}

//调用bufferevent_write后会触发该事件，如果bufferevent_setcb中设置了bufferevent_data_cb的话
void TcpServer::write_cb(struct bufferevent *bev, void *arg)
{
   //do nothing
}

void TcpServer::do_accept(evutil_socket_t listener, short eventVal, void *arg)
{
    ListenParam *cbparameter = (ListenParam*)arg;
    struct event_base *base = cbparameter->ListenBase;
    evutil_socket_t fd;
    struct sockaddr_in sin;
    socklen_t slen = sizeof(sin);
    fd = accept(listener, (struct sockaddr *)&sin, &slen);
    if (fd < 0) {
        perror("accept");
        return;
    }

    Tprintf(L"ACCEPT: fd = %u\n", fd);
#if defined (WIN32)
    evthread_use_windows_threads();
#else
    evthread_use_pthreads();
#endif
    evthread_make_base_notifiable(base);
    class SolarTcpIpPacket *pPacket = new class SolarTcpIpPacket();
    cbparameter->pSvr->m_mapPacket[fd] = pPacket;
    pPacket->StartParseThread();
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
    m_VecBev.push_back(bev);
    bufferevent_setcb(bev, read_cb, NULL, event_cb, cbparameter->pSvr);
    bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST);
}

//从socket读
void TcpServer::read_cb(struct bufferevent *bev, void *arg)
{
    TcpServer *pSvr = (TcpServer*)arg;
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
        pSvr->DealWithData(bev,cbData, n);
    }
}

void TcpServer::RemoveFromVec(struct bufferevent *bev)
{
    for (vector<struct bufferevent*>::iterator it = m_VecBev.begin(); it != m_VecBev.end(); it++)
    {
        if ((*it) == bev)
        {
            m_VecBev.erase(it);
            break;
        }
    }
}

void TcpServer::event_cb(struct bufferevent *bev, short event, void *arg)
{
    TcpServer *pSvr = (TcpServer*)arg;
    evutil_socket_t fd = bufferevent_getfd(bev);
    Tprintf(L"fd = %u, ", fd);
    if (event & BEV_EVENT_TIMEOUT)
    {
        Tprintf(L"Timed out\n"); //if bufferevent_set_timeouts() called
    }
    else if (event & BEV_EVENT_EOF) 
    {
        Tprintf(L"connection closed\n");
        pSvr->RemoveFromVec(bev);
        if (pSvr->m_mapPacket.find(fd) != pSvr->m_mapPacket.end())
        {
            delete pSvr->m_mapPacket[fd];
            pSvr->m_mapPacket.erase(fd);
        }
        bufferevent_free(bev);
    }
    else if (event & BEV_EVENT_ERROR)
    {
        Tprintf(L"some other error\n");
        pSvr->RemoveFromVec(bev);
        if (pSvr->m_mapPacket.find(fd) != pSvr->m_mapPacket.end())
        {
            delete pSvr->m_mapPacket[fd];
            pSvr->m_mapPacket.erase(fd);
        }
        bufferevent_free(bev);
    }
}

bool TcpServer::SendData(const char* cIP, int nPort, const char *pData, int nLen)
{
    struct bufferevent* bev = GetBufferEvent(cIP, nPort);
    if (bev == NULL)
        return false;
    if (bufferevent_write(bev, pData, nLen) < 0)
        return false;
    else
        return true;
}

struct bufferevent* TcpServer::GetBufferEvent(const char* cIP, int nPort)
{
    struct bufferevent* bev = NULL;
    char cIPParse[16];
    for (vector<struct bufferevent*>::iterator it = m_VecBev.begin(); it != m_VecBev.end(); it++)
    {
        evutil_socket_t fd = bufferevent_getfd(*it);
        struct sockaddr_in sin;
        int nLen = sizeof(SOCKADDR);
        if (getpeername(fd, (struct sockaddr*)&sin, &nLen) != 0)
            continue;//遇到无效的fd,继续
        inet_ntop(AF_INET, (void *)&sin.sin_addr, cIPParse, 16);
        if (strcmp(cIPParse, cIP) == 0 && nPort == ntohs(sin.sin_port))
        {
            bev = (*it);
            break;
        }
    }

    return bev;
}

//解析从指定client发出的数据
bool TcpServer::DealWithData(struct bufferevent *bev, const char*pData, int nLen)
{
    evutil_socket_t fd = bufferevent_getfd(bev);
    if (m_mapPacket.find(fd) == m_mapPacket.end())
        return false;
    struct sockaddr_in sin;
    int nsocketLen = sizeof(SOCKADDR);
    char cIPParse[16];
    ushort port;
    if (getpeername(fd, (struct sockaddr*)&sin, &nsocketLen) == 0)
    {
        inet_ntop(AF_INET, (void *)&sin.sin_addr, cIPParse, 16);
        port = ntohs(sin.sin_port);
    }

    m_mapPacket[fd]->AppendRecvData(nLen, (byte*)pData);
    SolarPacket* pPacketData = m_mapPacket[fd]->PullRecvPacket();
    if (pPacketData != NULL)
    {
        switch (pPacketData->byteCmd)
        {
        case 0x10:
            Tprintf(L"recv 0x10 response");
            break;
        case 0x11:
            break;
        case 0x12:
            break;
        case 0x13:
        {
            int nRecvlen = pPacketData->nDataLen;
            CCDParam param;
            memcpy(&param, pPacketData->m_Data, sizeof(CCDParam));
            Tprintf(L"recv CCD Param: %d", param.nSawMarkGroove);
        }
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
