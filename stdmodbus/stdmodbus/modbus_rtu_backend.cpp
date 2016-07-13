#include "stdafx.h"
#include "modbus_rtu_backend.h"

#include "modbus-rtu-private.h"
#include "modbus_common.h"

modbus_rtu_backend::modbus_rtu_backend()
{
    backend_type = _MODBUS_BACKEND_TYPE_RTU;
    header_length = _MODBUS_RTU_HEADER_LENGTH;
    checksum_length = _MODBUS_RTU_CHECKSUM_LENGTH;
    max_adu_length = MODBUS_RTU_MAX_ADU_LENGTH;
}


modbus_rtu_backend::~modbus_rtu_backend()
{
}

/* Define the slave ID of the remote device to talk in master mode or set the
* internal slave ID in slave mode */
int modbus_rtu_backend::modbus_set_slave(modbus_t*ctx, int slave)
{
    /* Broadcast address is 0 (MODBUS_BROADCAST_ADDRESS) */
    if (slave >= 0 && slave <= 247) {
        ctx->slave = slave;
    }
    else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

/* Builds a RTU request header */
int modbus_rtu_backend::modbus_build_request_basis(modbus_t *ctx, int function, int addr, int nb, uint8_t *req)
{
    assert(ctx->slave != -1);
    req[0] = ctx->slave;
    req[1] = function;
    req[2] = addr >> 8;
    req[3] = addr & 0x00ff;
    req[4] = nb >> 8;
    req[5] = nb & 0x00ff;

    return _MODBUS_RTU_PRESET_REQ_LENGTH;
}

/* Builds a RTU response header */
int modbus_rtu_backend::modbus_build_response_basis(sft_t *sft, uint8_t *rsp)
{
    /* In this case, the slave is certainly valid because a check is already
    * done in _modbus_rtu_listen */
    rsp[0] = sft->slave;
    rsp[1] = sft->function;

    return _MODBUS_RTU_PRESET_RSP_LENGTH;
}

int modbus_rtu_backend::modbus_prepare_response_tid(const uint8_t *req, int *req_length)
{
    (*req_length) -= _MODBUS_RTU_CHECKSUM_LENGTH;
    /* No TID */
    return 0;
}

int modbus_rtu_backend::modbus_send_msg_pre(uint8_t *req, int req_length)
{
    uint16_t crc = modbus_common::crc16(req, req_length);
    req[req_length++] = crc >> 8;
    req[req_length++] = crc & 0x00FF;

    return req_length;
}

/* This simple implementation is sort of a substitute of the select() call,
* working this way: the win32_ser_select() call tries to read some data from
* the serial port, setting the timeout as the select() call would. Data read is
* stored into the receive buffer, that is then consumed by the win32_ser_read()
* call.  So win32_ser_select() does both the event waiting and the reading,
* while win32_ser_read() only consumes the receive buffer.
*/

void modbus_rtu_backend::win32_ser_init(struct win32_ser *ws)
{
    /* Clear everything */
    memset(ws, 0x00, sizeof(struct win32_ser));

    /* Set file handle to invalid */
    ws->fd = INVALID_HANDLE_VALUE;
}

/* FIXME Try to remove length_to_read -> max_len argument, only used by win32 */
int modbus_rtu_backend::win32_ser_select(struct win32_ser *ws, int max_len, const struct timeval *tv)
{
    COMMTIMEOUTS comm_to;
    unsigned int msec = 0;

    /* Check if some data still in the buffer to be consumed */
    if (ws->n_bytes > 0) {
        return 1;
    }

    /* Setup timeouts like select() would do.
    FIXME Please someone on Windows can look at this?
    Does it possible to use WaitCommEvent?
    When tv is NULL, MAXDWORD isn't infinite!
    */
    if (tv == NULL) {
        msec = MAXDWORD;
    }
    else {
        msec = tv->tv_sec * 1000 + tv->tv_usec / 1000;
        if (msec < 1)
            msec = 1;
    }

    comm_to.ReadIntervalTimeout = msec;
    comm_to.ReadTotalTimeoutMultiplier = 0;
    comm_to.ReadTotalTimeoutConstant = msec;
    comm_to.WriteTotalTimeoutMultiplier = 0;
    comm_to.WriteTotalTimeoutConstant = 1000;
    SetCommTimeouts(ws->fd, &comm_to);

    /* Read some bytes */
    if ((max_len > PY_BUF_SIZE) || (max_len < 0)) {
        max_len = PY_BUF_SIZE;
    }

    if (ReadFile(ws->fd, &ws->buf, max_len, &ws->n_bytes, NULL)) {
        /* Check if some bytes available */
        if (ws->n_bytes > 0) {
            /* Some bytes read */
            return 1;
        }
        else {
            /* Just timed out */
            return 0;
        }
    }
    else {
        /* Some kind of error */
        return -1;
    }
}

int modbus_rtu_backend::win32_ser_read(struct win32_ser *ws, uint8_t *p_msg, unsigned int max_len)
{
    unsigned int n = ws->n_bytes;

    if (max_len < n) {
        n = max_len;
    }

    if (n > 0) {
        memcpy(p_msg, ws->buf, n);
    }

    ws->n_bytes -= n;

    return n;
}

ssize_t modbus_rtu_backend::modbus_send(modbus_t *ctx, const uint8_t *req, int req_length)
{
    modbus_rtu_t *ctx_rtu = (modbus_rtu_t *)ctx->backend_data;
    DWORD n_bytes = 0;
    return (WriteFile(ctx_rtu->w_ser.fd, req, req_length, &n_bytes, NULL)) ? (ssize_t)n_bytes : -1;
}

int modbus_rtu_backend::modbus_receive(modbus_t *ctx, uint8_t *req)
{
    int rc;
    modbus_rtu_t *ctx_rtu = (modbus_rtu_t *)ctx->backend_data;

    if (ctx_rtu->confirmation_to_ignore) {
        modbus_common::receive_msg(ctx, req, MSG_CONFIRMATION, this);
        /* Ignore errors and reset the flag */
        ctx_rtu->confirmation_to_ignore = FALSE;
        rc = 0;
        if (ctx->debug) {
            printf("Confirmation to ignore\n");
        }
    }
    else {
        rc = modbus_common::receive_msg(ctx, req, MSG_INDICATION,this);
        if (rc == 0) {
            /* The next expected message is a confirmation to ignore */
            ctx_rtu->confirmation_to_ignore = TRUE;
        }
    }
    return rc;
}

ssize_t modbus_rtu_backend::modbus_recv(modbus_t *ctx, uint8_t *rsp, int rsp_length)
{
    return win32_ser_read(&((modbus_rtu_t *)ctx->backend_data)->w_ser, rsp, rsp_length);
}

/* The check_crc16 function shall return 0 is the message is ignored and the
message length if the CRC is valid. Otherwise it shall return -1 and set
errno to EMBADCRC. */
int modbus_rtu_backend::modbus_check_integrity(modbus_t *ctx, uint8_t *msg, const int msg_length)
{
    uint16_t crc_calculated;
    uint16_t crc_received;
    int slave = msg[0];

    /* Filter on the Modbus unit identifier (slave) in RTU mode to avoid useless
    * CRC computing. */
    if (slave != ctx->slave && slave != MODBUS_BROADCAST_ADDRESS) {
        if (ctx->debug) {
            printf("Request for slave %d ignored (not %d)\n", slave, ctx->slave);
        }
        /* Following call to check_confirmation handles this error */
        return 0;
    }

    crc_calculated = modbus_common::crc16(msg, msg_length - 2);
    crc_received = (msg[msg_length - 2] << 8) | msg[msg_length - 1];

    /* Check CRC of msg */
    if (crc_calculated == crc_received) {
        return msg_length;
    }
    else {
        if (ctx->debug) {
            fprintf(stderr, "ERROR CRC received 0x%0X != CRC calculated 0x%0X\n",
                crc_received, crc_calculated);
        }

        if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL) {
            modbus_flush(ctx);
        }
        errno = EMBBADCRC;
        return -1;
    }
}

int modbus_rtu_backend::modbus_pre_check_confirmation(modbus_t *ctx, const uint8_t *req, const uint8_t *rsp, int rsp_length)
{
    /* Check responding slave is the slave we requested (except for broacast
    * request) */
    if (req[0] != rsp[0] && req[0] != MODBUS_BROADCAST_ADDRESS) {
        if (ctx->debug) {
            fprintf(stderr,
                "The responding slave %d isn't the requested slave %d\n",
                rsp[0], req[0]);
        }
        errno = EMBBADSLAVE;
        return -1;
    }
    else {
        return 0;
    }
}

int modbus_rtu_backend::modbus_connect(modbus_t *ctx)
{
    DCB dcb;
    modbus_rtu_t *ctx_rtu = (modbus_rtu_t *)ctx->backend_data;

    if (ctx->debug) {
        printf("Opening %s at %d bauds (%c, %d, %d)\n",
            ctx_rtu->device, ctx_rtu->baud, ctx_rtu->parity,
            ctx_rtu->data_bit, ctx_rtu->stop_bit);
    }

    win32_ser_init(&ctx_rtu->w_ser);

    /* ctx_rtu->device should contain a string like "COMxx:" xx being a decimal
    * number */
    ctx_rtu->w_ser.fd = CreateFileA(ctx_rtu->device,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    /* Error checking */
    if (ctx_rtu->w_ser.fd == INVALID_HANDLE_VALUE) {
        if (ctx->debug) {
            fprintf(stderr, "ERROR Can't open the device %s (LastError %d)\n",
                ctx_rtu->device, (int)GetLastError());
        }
        return -1;
    }

    /* Save params */
    ctx_rtu->old_dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(ctx_rtu->w_ser.fd, &ctx_rtu->old_dcb)) {
        if (ctx->debug) {
            fprintf(stderr, "ERROR Error getting configuration (LastError %d)\n",
                (int)GetLastError());
        }
        CloseHandle(ctx_rtu->w_ser.fd);
        ctx_rtu->w_ser.fd = INVALID_HANDLE_VALUE;
        return -1;
    }

    /* Build new configuration (starting from current settings) */
    dcb = ctx_rtu->old_dcb;

    /* Speed setting */
    switch (ctx_rtu->baud) {
    case 110:
        dcb.BaudRate = CBR_110;
        break;
    case 300:
        dcb.BaudRate = CBR_300;
        break;
    case 600:
        dcb.BaudRate = CBR_600;
        break;
    case 1200:
        dcb.BaudRate = CBR_1200;
        break;
    case 2400:
        dcb.BaudRate = CBR_2400;
        break;
    case 4800:
        dcb.BaudRate = CBR_4800;
        break;
    case 9600:
        dcb.BaudRate = CBR_9600;
        break;
    case 14400:
        dcb.BaudRate = CBR_14400;
        break;
    case 19200:
        dcb.BaudRate = CBR_19200;
        break;
    case 38400:
        dcb.BaudRate = CBR_38400;
        break;
    case 57600:
        dcb.BaudRate = CBR_57600;
        break;
    case 115200:
        dcb.BaudRate = CBR_115200;
        break;
    case 230400:
        /* CBR_230400 - not defined */
        dcb.BaudRate = 230400;
        break;
    case 250000:
        dcb.BaudRate = 250000;
        break;
    case 460800:
        dcb.BaudRate = 460800;
        break;
    case 500000:
        dcb.BaudRate = 500000;
        break;
    case 921600:
        dcb.BaudRate = 921600;
        break;
    case 1000000:
        dcb.BaudRate = 1000000;
        break;
    default:
        dcb.BaudRate = CBR_9600;
        if (ctx->debug) {
            fprintf(stderr, "WARNING Unknown baud rate %d for %s (B9600 used)\n",
                ctx_rtu->baud, ctx_rtu->device);
        }
    }

    /* Data bits */
    switch (ctx_rtu->data_bit) {
    case 5:
        dcb.ByteSize = 5;
        break;
    case 6:
        dcb.ByteSize = 6;
        break;
    case 7:
        dcb.ByteSize = 7;
        break;
    case 8:
    default:
        dcb.ByteSize = 8;
        break;
    }

    /* Stop bits */
    if (ctx_rtu->stop_bit == 1)
        dcb.StopBits = ONESTOPBIT;
    else /* 2 */
        dcb.StopBits = TWOSTOPBITS;

    /* Parity */
    if (ctx_rtu->parity == 'N') {
        dcb.Parity = NOPARITY;
        dcb.fParity = FALSE;
    }
    else if (ctx_rtu->parity == 'E') {
        dcb.Parity = EVENPARITY;
        dcb.fParity = TRUE;
    }
    else {
        /* odd */
        dcb.Parity = ODDPARITY;
        dcb.fParity = TRUE;
    }

    /* Hardware handshaking left as default settings retrieved */

    /* No software handshaking */
    dcb.fTXContinueOnXoff = TRUE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;

    /* Binary mode (it's the only supported on Windows anyway) */
    dcb.fBinary = TRUE;

    /* Don't want errors to be blocking */
    dcb.fAbortOnError = FALSE;

    /* Setup port */
    if (!SetCommState(ctx_rtu->w_ser.fd, &dcb)) {
        if (ctx->debug) {
            fprintf(stderr, "ERROR Error setting new configuration (LastError %d)\n",
                (int)GetLastError());
        }
        CloseHandle(ctx_rtu->w_ser.fd);
        ctx_rtu->w_ser.fd = INVALID_HANDLE_VALUE;
        return -1;
    }

    return 0;
}

void modbus_rtu_backend::modbus_close(modbus_t *ctx)
{
    /* Restore line settings and close file descriptor in RTU mode */
    modbus_rtu_t *ctx_rtu = (modbus_rtu_t *)ctx->backend_data;
    /* Revert settings */
    if (!SetCommState(ctx_rtu->w_ser.fd, &ctx_rtu->old_dcb) && ctx->debug) {
        fprintf(stderr, "ERROR Couldn't revert to configuration (LastError %d)\n",
            (int)GetLastError());
    }

    if (!CloseHandle(ctx_rtu->w_ser.fd) && ctx->debug) {
        fprintf(stderr, "ERROR Error while closing handle (LastError %d)\n",
            (int)GetLastError());
    }
}

int modbus_rtu_backend::modbus_flush(modbus_t *ctx)
{
    modbus_rtu_t *ctx_rtu = (modbus_rtu_t *)ctx->backend_data;
    ctx_rtu->w_ser.n_bytes = 0;
    return (PurgeComm(ctx_rtu->w_ser.fd, PURGE_RXCLEAR) == FALSE);
}

int modbus_rtu_backend::modbus_select(modbus_t *ctx, fd_set *rset, struct timeval *tv, int length_to_read)
{
    int s_rc;
    s_rc = win32_ser_select(&((modbus_rtu_t *)ctx->backend_data)->w_ser, length_to_read, tv);
    if (s_rc == 0) {
        errno = ETIMEDOUT;
        return -1;
    }

    if (s_rc < 0) {
        return -1;
    }
    return s_rc;
}

void modbus_rtu_backend::modbus_free(modbus_t *ctx)
{
    free(((modbus_rtu_t*)ctx->backend_data)->device);
    free(ctx->backend_data);
    free(ctx);
}

