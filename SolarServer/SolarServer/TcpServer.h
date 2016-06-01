#pragma once
#include <vector>
using std::vector; 

#include "event2\util.h"

typedef struct _tagCCDPar
{
    ushort nSawMarkGroove;
    ushort SawMarkStep;
    ushort SawMarkEdge;
    ushort sori;
    ushort bow;
    ushort chips;
    ushort breakages;
    ushort holes;
    ushort cracks;
    ushort NVCD;//����
    ushort stain; //����
    ushort glue; //
}CCDParam;

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();

public:
    bool Init(int nPort);
    bool SendData(const char* cIP, int nPort, const char *pData, int nLen);


protected:
    bool DealWithData(struct bufferevent *bev, const char*pData, int nLen);
    void TcpServer::RemoveFromVec(struct bufferevent *bev);
    struct bufferevent* GetBufferEvent(const char* cIP, int nPort);

public:
    static vector<struct bufferevent*> m_VecBev;

private:
    class SolarTcpIpPacket *m_pPacket;
    int m_nListenPort;
    static void* ListenThread(void *pParam);
    static void do_accept(evutil_socket_t listener, short eventVal, void *arg);
    static void read_cb(struct bufferevent *bev, void *arg);
    static void event_cb(struct bufferevent *bev, short event, void *arg);
    static void write_cb(struct bufferevent *bev, void *arg);
};

