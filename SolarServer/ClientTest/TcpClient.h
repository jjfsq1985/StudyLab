#pragma once
#include "ClientEmulation.h"

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();

public:
    bool Init(const char *cIPAddr, int nPort);
    bool SendData(const char *pData, int nLen);

public:
    bool m_bConnected;

protected:
    bool m_bConnecting;
    char m_cIPAddr[16];
    int m_nPort;

protected:
    bool DealWithData(struct bufferevent *bev,const char*pData, int nLen);

private:
    class ClientEmulation *m_pEmulation;
    class SolarTcpIpPacket *m_pPacket;
    static struct bufferevent *m_pBev;

private:
    static void* ConnectThread(void *pParam);
    static void read_cb(struct bufferevent *bev, void *arg);
    static void event_cb(struct bufferevent *bev, short event, void *arg);
    static void write_cb(struct bufferevent *bev, void *arg);
};

