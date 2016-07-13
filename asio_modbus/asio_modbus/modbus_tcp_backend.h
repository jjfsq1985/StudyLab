#pragma once
#include "interface_backend.h"

# include <winsock2.h>
# include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#if !defined(ECONNRESET)
#define ECONNRESET   WSAECONNRESET
#endif
#if !defined(ECONNREFUSED)
#define ECONNREFUSED WSAECONNREFUSED
#endif
#if !defined(ETIMEDOUT)
#define ETIMEDOUT    WSAETIMEDOUT
#endif
#if !defined(ENOPROTOOPT)
#define ENOPROTOOPT  WSAENOPROTOOPT
#endif
#if !defined(EINPROGRESS)
#define EINPROGRESS  WSAEINPROGRESS
#endif

#define MODBUS_TCP_DEFAULT_PORT   502
#define MODBUS_TCP_SLAVE         0xFF

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
* TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes
*/
#define MODBUS_TCP_MAX_ADU_LENGTH  260

class modbus_tcp_backend : public interface_backend
{
public:
    modbus_tcp_backend();
    virtual ~modbus_tcp_backend();

public:
    int tcp_init_win32();
    int tcp_set_ipv4_options(int s);
    int connect_net(int sockfd, const struct sockaddr *addr, socklen_t addrlen, const struct timeval *ro_tv);

public:
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
    virtual int modbus_select(modbus_t *ctx, fd_set *rset, struct timeval *tv, int msg_length);
    virtual void modbus_free(modbus_t *ctx);
};

