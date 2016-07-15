#include "stdafx.h"
#include "modbus_tcp_backend.h"

#include "modbus-tcp-private.h"
#include "modbus_common.h"

modbus_tcp_backend * modbus_tcp_backend::m_pInstance = NULL;

modbus_tcp_backend::modbus_tcp_backend()
{
    backend_type = _MODBUS_BACKEND_TYPE_TCP;
    header_length = _MODBUS_TCP_HEADER_LENGTH;
    checksum_length = _MODBUS_TCP_CHECKSUM_LENGTH;
    max_adu_length = MODBUS_TCP_MAX_ADU_LENGTH;
    modbus_common::tcp_init_win32();
}

modbus_tcp_backend * modbus_tcp_backend::GetInstance()
{
    if (m_pInstance == NULL)  //判断是否第一次调用 
    {
        //实现线程安全，用Singleton实现异常安全  
        //Singleton析构总是发生的无论是因为异常抛出还是语句块结束。  
        Singleton sLock(modbus_lock_cs);

        if (m_pInstance == NULL)
            m_pInstance = new modbus_tcp_backend();
    }
    return m_pInstance;
}


modbus_tcp_backend::~modbus_tcp_backend()
{
    if (modbus_tcp_backend::m_pInstance)
        delete modbus_tcp_backend::m_pInstance;
}

unsigned int modbus_tcp_backend::modbus_backend_type()
{
    return backend_type;
}

unsigned int modbus_tcp_backend::modbus_header_length()
{
    return header_length;
}

unsigned int modbus_tcp_backend::modbus_checksum_length()
{
    return checksum_length;
}

unsigned int modbus_tcp_backend::modbus_max_adu_length()
{
    return max_adu_length;
}

int modbus_tcp_backend::modbus_set_slave(modbus_t*ctx, int slave)
{
    /* Broadcast address is 0 (MODBUS_BROADCAST_ADDRESS) */
    if (slave >= 0 && slave <= 247) {
        ctx->slave = slave;
    }
    else if (slave == MODBUS_TCP_SLAVE) {
        /* The special value MODBUS_TCP_SLAVE (0xFF) can be used in TCP mode to
        * restore the default value. */
        ctx->slave = slave;
    }
    else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

/* Builds a TCP request header */
int modbus_tcp_backend::modbus_build_request_basis(modbus_t *ctx, int function, int addr, int nb, uint8_t *req)
{
    modbus_tcp_t *ctx_tcp = (modbus_tcp_t *)(ctx->backend_data);

    /* Increase transaction ID */
    if (ctx_tcp->t_id < UINT16_MAX)
        ctx_tcp->t_id++;
    else
        ctx_tcp->t_id = 0;
    req[0] = ctx_tcp->t_id >> 8;
    req[1] = ctx_tcp->t_id & 0x00ff;

    /* Protocol Modbus */
    req[2] = 0;
    req[3] = 0;

    /* Length will be defined later by set_req_length_tcp at offsets 4
     and 5 */

    req[6] = ctx->slave;
    req[7] = function;
    req[8] = addr >> 8;
    req[9] = addr & 0x00ff;
    req[10] = nb >> 8;
    req[11] = nb & 0x00ff;

    return _MODBUS_TCP_PRESET_REQ_LENGTH;
}

/* Builds a TCP response header */
int modbus_tcp_backend::modbus_build_response_basis(sft_t *sft, uint8_t *rsp)
{
    /* Extract from MODBUS Messaging on TCP/IP Implementation
    Guide V1.0b (page 23/46):
    The transaction identifier is used to associate the future
    response with the request. */
    rsp[0] = sft->t_id >> 8;
    rsp[1] = sft->t_id & 0x00ff;

    /* Protocol Modbus */
    rsp[2] = 0;
    rsp[3] = 0;

    /* Length will be set later by send_msg (4 and 5) */

    /* The slave ID is copied from the indication */
    rsp[6] = sft->slave;
    rsp[7] = sft->function;

    return _MODBUS_TCP_PRESET_RSP_LENGTH;
}

int modbus_tcp_backend::modbus_prepare_response_tid(const uint8_t *req, int *req_length)
{
    return (req[0] << 8) + req[1];
}

int modbus_tcp_backend::modbus_send_msg_pre(uint8_t *req, int req_length)
{
    /* Substract the header length to the message length */
    int mbap_length = req_length - 6;

    req[4] = mbap_length >> 8;
    req[5] = mbap_length & 0x00FF;

    return req_length;
}

ssize_t modbus_tcp_backend::modbus_send(modbus_t *ctx, const uint8_t *req, int req_length)
{
    return send(ctx->s, (const char *)req, req_length, 0);
}

int modbus_tcp_backend::modbus_receive(modbus_t *ctx, uint8_t *req)
{
    return modbus_common::receive_msg(ctx, req, MSG_INDICATION,this);
}

ssize_t modbus_tcp_backend::modbus_recv(modbus_t *ctx, uint8_t *rsp, int rsp_length)
{
    return recv(ctx->s, (char *)rsp, rsp_length, 0);
}

int modbus_tcp_backend::modbus_check_integrity(modbus_t *ctx, uint8_t *msg, const int msg_length)
{
    return msg_length;
}

int modbus_tcp_backend::modbus_pre_check_confirmation(modbus_t *ctx, const uint8_t *req, const uint8_t *rsp, int rsp_length)
{
    /* Check transaction ID */
    if (req[0] != rsp[0] || req[1] != rsp[1]) {
        if (ctx->debug) {
            fprintf(stderr, "Invalid transaction ID received 0x%X (not 0x%X)\n",
                (rsp[0] << 8) + rsp[1], (req[0] << 8) + req[1]);
        }
        errno = EMBBADDATA;
        return -1;
    }

    /* Check protocol ID */
    if (rsp[2] != 0x0 && rsp[3] != 0x0) {
        if (ctx->debug) {
            fprintf(stderr, "Invalid protocol ID received 0x%X (not 0x0)\n",
                (rsp[2] << 8) + rsp[3]);
        }
        errno = EMBBADDATA;
        return -1;
    }

    return 0;
}

int modbus_tcp_backend::tcp_set_ipv4_options(int s)
{
    int rc;
    int option;

    /* Set the TCP no delay flag */
    /* SOL_TCP = IPPROTO_TCP */
    option = 1;
    rc = setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
        (const char *)&option, sizeof(int));
    if (rc == -1) {
        return -1;
    }

    /* If the OS does not offer SOCK_NONBLOCK, fall back to setting FIONBIO to
    * make sockets non-blocking */
    /* Do not care about the return value, this is optional */
#if !defined(SOCK_NONBLOCK) && defined(FIONBIO)
    {
        /* Setting FIONBIO expects an unsigned long according to MSDN */
        u_long loption = 1;
        ioctlsocket(s, FIONBIO, &loption);
    }
#endif

    return 0;
}

int modbus_tcp_backend::connect_net(int sockfd, const struct sockaddr *addr, socklen_t addrlen, const struct timeval *ro_tv)
{
    int rc = connect(sockfd, addr, addrlen);

    int wsaError = 0;
    if (rc == -1) {
        wsaError = WSAGetLastError();
    }

    if (wsaError == WSAEWOULDBLOCK || wsaError == WSAEINPROGRESS) {
        fd_set wset;
        int optval;
        socklen_t optlen = sizeof(optval);
        struct timeval tv = *ro_tv;

        /* Wait to be available in writing */
        FD_ZERO(&wset);
        FD_SET(sockfd, &wset);
        rc = select(sockfd + 1, NULL, &wset, NULL, &tv);
        if (rc <= 0) {
            /* Timeout or fail */
            return -1;
        }

        /* The connection is established if SO_ERROR and optval are set to 0 */
        rc = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&optval, &optlen);
        if (rc == 0 && optval == 0) {
            return 0;
        }
        else {
            errno = ECONNREFUSED;
            return -1;
        }
    }
    return rc;
}

int modbus_tcp_backend::modbus_connect(modbus_t *ctx)
{
    int rc;
    /* Specialized version of sockaddr for Internet socket address (same size) */
    struct sockaddr_in addr;
    modbus_tcp_t *ctx_tcp = (modbus_tcp_t *)ctx->backend_data;
    int flags = SOCK_STREAM;

    if (modbus_common::tcp_init_win32() == -1) {
        return -1;
    }

    ctx->s = socket(PF_INET, flags, 0);
    if (ctx->s == -1) {
        return -1;
    }

    rc = tcp_set_ipv4_options(ctx->s);
    if (rc == -1) {
        closesocket(ctx->s);
        ctx->s = -1;
        return -1;
    }

    if (ctx->debug) {
        printf("Connecting to %s:%d\n", ctx_tcp->ip, ctx_tcp->port);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(ctx_tcp->port);
    inet_pton(AF_INET, ctx_tcp->ip, (void*)&addr.sin_addr);
    rc = connect_net(ctx->s, (struct sockaddr *)&addr, sizeof(addr), &ctx->response_timeout);
    if (rc == -1) {
        closesocket(ctx->s);
        ctx->s = -1;
        return -1;
    }

    return 0;
}

void modbus_tcp_backend::modbus_close(modbus_t *ctx)
{
    if (ctx->s != -1) {
        shutdown(ctx->s, SD_BOTH);
        closesocket(ctx->s);
        ctx->s = -1;
    }
}

int modbus_tcp_backend::modbus_flush(modbus_t *ctx)
{
    int rc;
    int rc_sum = 0;

    do {
        /* Extract the garbage from the socket */
        char devnull[MODBUS_TCP_MAX_ADU_LENGTH];
        /* On Win32, it's a bit more complicated to not wait */
        fd_set rset;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&rset);
        FD_SET(ctx->s, &rset);
        rc = select(ctx->s + 1, &rset, NULL, NULL, &tv);
        if (rc == -1) {
            return -1;
        }

        if (rc == 1) {
            /* There is data to flush */
            rc = recv(ctx->s, devnull, MODBUS_TCP_MAX_ADU_LENGTH, 0);
        }
        if (rc > 0) {
            rc_sum += rc;
        }
    } while (rc == MODBUS_TCP_MAX_ADU_LENGTH);

    return rc_sum;
}

int modbus_tcp_backend::modbus_select(modbus_t *ctx, fd_set *rset, struct timeval *tv, int msg_length)
{
    int s_rc;
    while ((s_rc = select(ctx->s + 1, rset, NULL, NULL, tv)) == -1) {
        if (errno == EINTR) {
            if (ctx->debug) {
                fprintf(stderr, "A non blocked signal was caught\n");
            }
            /* Necessary after an error */
            FD_ZERO(rset);
            FD_SET(ctx->s, rset);
        }
        else {
            return -1;
        }
    }

    if (s_rc == 0) {
        errno = ETIMEDOUT;
        return -1;
    }

    return s_rc;
}

void modbus_tcp_backend::modbus_free(modbus_t *ctx)
{
    modbus_tcp_t *ctx_tcp = (modbus_tcp_t *)ctx->backend_data;
    delete ctx_tcp;
    delete ctx;
}