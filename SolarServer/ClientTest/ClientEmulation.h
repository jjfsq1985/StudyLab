#pragma once
#include "pthread.h"
#include <list>
using namespace std;

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
    ushort NVCD;//าþมั
    ushort stain; //ิเฮÛ
    ushort glue; //
}CCDParam;

class ClientEmulation
{
public:
    ClientEmulation(class TcpClient *pClient);
    ~ClientEmulation();

public:
    void StartWork();
    void StopWork();

    void CCDEmulation();
    void SendServerProc();


private:
    class TcpClient *m_pClient;
    CCDParam* PopCCDdata();
    static void* CCDdataEmulation(void * pParam);
    static void* SendSvr(void* pParam);
private:
    bool m_threadRun;
    pthread_t m_threadId;
    pthread_t m_SendId;
    pthread_mutex_t m_mutex;
    list<CCDParam*> m_lstccdpar;
    
};

