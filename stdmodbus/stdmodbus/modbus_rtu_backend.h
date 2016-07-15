#pragma once
#include "interface_backend.h"

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
* RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
*/
#define MODBUS_RTU_MAX_ADU_LENGTH  256
#define MODBUS_RTU_RS232 0
#define MODBUS_RTU_RS485 1
#define MODBUS_RTU_RTS_NONE   0
#define MODBUS_RTU_RTS_UP     1
#define MODBUS_RTU_RTS_DOWN   2

class modbus_rtu_backend : public interface_backend
{
private:
    modbus_rtu_backend();
    static modbus_rtu_backend * m_pInstance;
public:
    static modbus_rtu_backend * GetInstance();
    virtual ~modbus_rtu_backend();

public:
    void win32_ser_init(struct win32_ser *ws);
    int win32_ser_select(struct win32_ser *ws, int max_len, const struct timeval *tv);
    int win32_ser_read(struct win32_ser *ws, uint8_t *p_msg, unsigned int max_len);

public:
    virtual unsigned int modbus_backend_type();
    virtual unsigned int modbus_header_length();
    virtual unsigned int modbus_checksum_length();
    virtual unsigned int modbus_max_adu_length();
    virtual int modbus_set_slave(modbus_t*ctx, int slave);
    virtual int modbus_build_request_basis(modbus_t *ctx, int function, int addr, int nb, uint8_t *req);
    virtual int modbus_build_response_basis(sft_t *sft, uint8_t *rsp);
    virtual int modbus_prepare_response_tid(const uint8_t *req, int *req_length);
    virtual int modbus_send_msg_pre(uint8_t *req, int req_length);
    virtual ssize_t modbus_send(modbus_t *ctx, const uint8_t *req, int req_length);
    virtual int modbus_receive(modbus_t *ctx, uint8_t *req);
    virtual ssize_t modbus_recv(modbus_t *ctx, uint8_t *rsp, int rsp_length);
    virtual int modbus_check_integrity(modbus_t *ctx, uint8_t *msg, const int msg_length);
    virtual int modbus_pre_check_confirmation(modbus_t *ctx, const uint8_t *req, const uint8_t *rsp, int rsp_length);
    virtual int modbus_connect(modbus_t *ctx);
    virtual void modbus_close(modbus_t *ctx);
    virtual int modbus_flush(modbus_t *ctx);
    virtual int modbus_select(modbus_t *ctx, fd_set *rset, struct timeval *tv, int length_to_read);
    virtual void modbus_free(modbus_t *ctx);
};

