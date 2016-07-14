#pragma once

#include "modbus_def.h"
#include "modbus-private.h"

class Lock
{
private:
    CRITICAL_SECTION m_cs;

public:
    Lock()
    {
        InitializeCriticalSection(&m_cs);
    }
    ~Lock()
    {
        DeleteCriticalSection(&m_cs);
    }
    void Enter()
    {
        EnterCriticalSection(&m_cs);
    }

    void Leave()
    {
        LeaveCriticalSection(&m_cs);
    }
};

class Singleton
{
private:
    class Lock m_lock;

public:
    Singleton(Lock s_lock) : m_lock(s_lock)
    {
        m_lock.Enter();
    }

    ~Singleton()
    {
        m_lock.Leave();
    }
};

static Lock modbus_lock_cs; //线程安全，单例临界区

class interface_backend
{
public:
    interface_backend()
    {

    }

    virtual ~interface_backend()
    {

    }
public:
    virtual int modbus_set_slave(modbus_t*ctx, int slave) = 0;
    virtual int modbus_build_request_basis(modbus_t *ctx, int function, int addr, int nb, uint8_t *req) = 0;
    virtual int modbus_build_response_basis(sft_t *sft, uint8_t *rsp) = 0;
    virtual int modbus_prepare_response_tid(const uint8_t *req, int *req_length) = 0;
    virtual int modbus_send_msg_pre(uint8_t *req, int req_length) = 0;
    virtual ssize_t modbus_send(modbus_t *ctx, const uint8_t *req, int req_length) = 0;
    virtual int modbus_receive(modbus_t *ctx, uint8_t *req) = 0;
    virtual ssize_t modbus_recv(modbus_t *ctx, uint8_t *rsp, int rsp_length) = 0;
    virtual int modbus_check_integrity(modbus_t *ctx, uint8_t *msg, const int msg_length) = 0;
    virtual int modbus_pre_check_confirmation(modbus_t *ctx, const uint8_t *req, const uint8_t *rsp, int rsp_length) = 0;
    virtual int modbus_connect(modbus_t *ctx) = 0;
    virtual void modbus_close(modbus_t *ctx) = 0;
    virtual int modbus_flush(modbus_t *ctx) = 0;
    virtual int modbus_select(modbus_t *ctx, fd_set *rset, struct timeval *tv, int length) = 0;
    virtual void modbus_free(modbus_t *ctx) = 0;

public:
    unsigned int backend_type;
    unsigned int header_length;
    unsigned int checksum_length;
    unsigned int max_adu_length;
};

