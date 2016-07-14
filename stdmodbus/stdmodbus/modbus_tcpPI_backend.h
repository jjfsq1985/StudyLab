#pragma once
#include "modbus_tcp_backend.h"

//PI(Protocol Indepedent协议无关)
//IPv6
class modbus_tcpPI_backend : public modbus_tcp_backend
{
private:
    modbus_tcpPI_backend();
    static modbus_tcpPI_backend * m_pInstance;
public:    
    static modbus_tcpPI_backend * GetInstance();
    virtual ~modbus_tcpPI_backend();

public:
    virtual int modbus_connect(modbus_t *ctx);
};

