#pragma once
#include "modbus_tcp_backend.h"

//PI(Protocol IndepedentЭ���޹�)
//IPv6
class modbus_tcpPI_backend : public modbus_tcp_backend
{
public:
    modbus_tcpPI_backend();
    virtual ~modbus_tcpPI_backend();

public:
    virtual int modbus_connect(modbus_t *ctx);
};

