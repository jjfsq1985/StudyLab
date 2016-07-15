#include "stdafx.h"
#include "modbus_common.h"

#include <string>
using namespace std;

/* Internal use */
const int MSG_LENGTH_UNDEFINED = -1;

/* 3 steps are used to parse the query */
typedef enum {
    _STEP_FUNCTION,
    _STEP_META,
    _STEP_DATA
} _step_t;

/* Table of CRC values for high-order byte */
static const uint8_t table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const uint8_t table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


modbus_common::modbus_common()
{
}


modbus_common::~modbus_common()
{
}

int modbus_common::receive_msg(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type, interface_backend *pbackend)
{
    int rc;
    fd_set rset;
    struct timeval tv;
    struct timeval *p_tv;
    int length_to_read;
    int msg_length = 0;
    _step_t step;

    if (ctx->debug) {
        if (msg_type == MSG_INDICATION) {
            printf("Waiting for a indication...\n");
        }
        else {
            printf("Waiting for a confirmation...\n");
        }
    }

    /* Add a file descriptor to the set */
    FD_ZERO(&rset);
    FD_SET(ctx->s, &rset);

    /* We need to analyse the message step by step.  At the first step, we want
    * to reach the function code because all packets contain this
    * information. */
    step = _STEP_FUNCTION;
    length_to_read = pbackend->modbus_header_length() + 1;

    if (msg_type == MSG_INDICATION) {
        /* Wait for a message, we don't know when the message will be
        * received */
        p_tv = NULL;
    }
    else {
        tv.tv_sec = ctx->response_timeout.tv_sec;
        tv.tv_usec = ctx->response_timeout.tv_usec;
        p_tv = &tv;
    }

    while (length_to_read != 0) {
        rc = pbackend->modbus_select(ctx, &rset, p_tv, length_to_read);
        if (rc == -1) {
            error_print(ctx, "select");
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) {
                int saved_errno = errno;

                if (errno == ETIMEDOUT) {
                    sleep_response_timeout(ctx);
                    pbackend->modbus_flush(ctx);
                }
                else if (errno == EBADF) {
                    pbackend->modbus_close(ctx);
                    pbackend->modbus_connect(ctx);
                }
                errno = saved_errno;
            }
            return -1;
        }

        rc = pbackend->modbus_recv(ctx, msg + msg_length, length_to_read);
        if (rc == 0) {
            errno = ECONNRESET;
            rc = -1;
        }

        if (rc == -1) {
            error_print(ctx, "read");
            if ((ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) &&
                (errno == ECONNRESET || errno == ECONNREFUSED ||
                errno == EBADF)) {
                int saved_errno = errno;
                pbackend->modbus_close(ctx);
                pbackend->modbus_connect(ctx);
                /* Could be removed by previous calls */
                errno = saved_errno;
            }
            return -1;
        }

        /* Display the hex code of each character received */
        if (ctx->debug) {
            int i;
            for (i = 0; i < rc; i++)
                printf("<%.2X>", msg[msg_length + i]);
        }

        /* Sums bytes received */
        msg_length += rc;
        /* Computes remaining bytes */
        length_to_read -= rc;

        if (length_to_read == 0) {
            switch (step) 
            {
            case _STEP_FUNCTION:
                /* Function code position */
                length_to_read = compute_meta_length_after_function(
                    msg[pbackend->modbus_header_length()],
                    msg_type);
                if (length_to_read != 0) {
                    step = _STEP_META;
                    break;
                } /* else switches straight to the next step */
            case _STEP_META:
                length_to_read = compute_data_length_after_meta(ctx, msg, msg_type, pbackend);
                if ((msg_length + length_to_read) > (int)pbackend->modbus_max_adu_length()) {
                    errno = EMBBADDATA;
                    error_print(ctx, "too many data");
                    return -1;
                }
                step = _STEP_DATA;
                break;
            default:
                break;
            }
        }

        if (length_to_read > 0 &&
            (ctx->byte_timeout.tv_sec > 0 || ctx->byte_timeout.tv_usec > 0)) {
            /* If there is no character in the buffer, the allowed timeout
            interval between two consecutive bytes is defined by
            byte_timeout */
            tv.tv_sec = ctx->byte_timeout.tv_sec;
            tv.tv_usec = ctx->byte_timeout.tv_usec;
            p_tv = &tv;
        }
        /* else timeout isn't set again, the full response must be read before
        expiration of response timeout (for CONFIRMATION only) */
    }

    if (ctx->debug)
        printf("\n");

    return pbackend->modbus_check_integrity(ctx, msg, msg_length);
}

int modbus_common::read_io_status(modbus_t *ctx, int function, int addr, int nb, uint8_t *dest, interface_backend *pbackend)
{
    int rc;
    int req_length;

    uint8_t req[_MIN_REQ_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];

    req_length = pbackend->modbus_build_request_basis(ctx, function, addr, nb, req);

    rc = send_msg(ctx, req, req_length, pbackend);
    if (rc > 0) {
        int i, temp, bit;
        int pos = 0;
        int offset;
        int offset_end;

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION, pbackend);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc, pbackend);
        if (rc == -1)
            return -1;

        offset = pbackend->modbus_header_length() + 2;
        offset_end = offset + rc;
        for (i = offset; i < offset_end; i++) {
            /* Shift reg hi_byte to temp */
            temp = rsp[i];

            for (bit = 0x01; (bit & 0xff) && (pos < nb);) {
                dest[pos++] = (temp & bit) ? TRUE : FALSE;
                bit = bit << 1;
            }

        }
    }

    return rc;
}

int modbus_common::check_confirmation(modbus_t *ctx, uint8_t *req, uint8_t *rsp, int rsp_length, interface_backend *pbackend)
{
    int rc;
    int rsp_length_computed;
    const int offset = pbackend->modbus_header_length();
    const int function = rsp[offset];

    if (pbackend) {
        rc = pbackend->modbus_pre_check_confirmation(ctx, req, rsp, rsp_length);
        if (rc == -1) {
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL) {
                sleep_response_timeout(ctx);
                pbackend->modbus_flush(ctx);
            }
            return -1;
        }
    }

    rsp_length_computed = compute_response_length_from_request(ctx, req,pbackend);

    /* Exception code */
    if (function >= 0x80) {
        if (rsp_length == (offset + 2 + (int)pbackend->modbus_checksum_length()) &&
            req[offset] == (rsp[offset] - 0x80)) {
            /* Valid exception code received */

            int exception_code = rsp[offset + 1];
            if (exception_code < MODBUS_EXCEPTION_MAX) {
                errno = MODBUS_ENOBASE + exception_code;
            }
            else {
                errno = EMBBADEXC;
            }
            error_print(ctx, NULL);
            return -1;
        }
        else {
            errno = EMBBADEXC;
            error_print(ctx, NULL);
            return -1;
        }
    }

    /* Check length */
    if ((rsp_length == rsp_length_computed ||
        rsp_length_computed == MSG_LENGTH_UNDEFINED) &&
        function < 0x80) {
        int req_nb_value;
        int rsp_nb_value;

        /* Check function code */
        if (function != req[offset]) {
            if (ctx->debug) {
                fprintf(stderr,
                    "Received function not corresponding to the request (0x%X != 0x%X)\n",
                    function, req[offset]);
            }
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL) {
                sleep_response_timeout(ctx);
                pbackend->modbus_flush(ctx);
            }
            errno = EMBBADDATA;
            return -1;
        }

        /* Check the number of values is corresponding to the request */
        switch (function) {
        case MODBUS_FC_READ_COILS:
        case MODBUS_FC_READ_DISCRETE_INPUTS:
            /* Read functions, 8 values in a byte (nb
            * of values in the request and byte count in
            * the response. */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
            rsp_nb_value = rsp[offset + 1];
            break;
        case MODBUS_FC_WRITE_AND_READ_REGISTERS:
        case MODBUS_FC_READ_HOLDING_REGISTERS:
        case MODBUS_FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 1] / 2);
            break;
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
            /* N Write functions */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
            break;
        case MODBUS_FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        default:
            /* 1 Write functions & others */
            req_nb_value = rsp_nb_value = 1;
        }

        if (req_nb_value == rsp_nb_value) {
            rc = rsp_nb_value;
        }
        else {
            if (ctx->debug) {
                fprintf(stderr,
                    "Quantity not corresponding to the request (%d != %d)\n",
                    rsp_nb_value, req_nb_value);
            }

            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL) {
                sleep_response_timeout(ctx);
                pbackend->modbus_flush(ctx);
            }

            errno = EMBBADDATA;
            rc = -1;
        }
    }
    else {
        if (ctx->debug) {
            fprintf(stderr,
                "Message length not corresponding to the computed length (%d != %d)\n",
                rsp_length, rsp_length_computed);
        }
        if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_PROTOCOL) {
            sleep_response_timeout(ctx);
            pbackend->modbus_flush(ctx);
        }
        errno = EMBBADDATA;
        rc = -1;
    }

    return rc;
}

int modbus_common::send_msg(modbus_t *ctx, uint8_t *msg, int msg_length, interface_backend *pbackend)
{
    int rc;
    int i;

    msg_length = pbackend->modbus_send_msg_pre(msg, msg_length);

    if (ctx->debug) {
        for (i = 0; i < msg_length; i++)
            printf("[%.2X]", msg[i]);
        printf("\n");
    }

    /* In recovery mode, the write command will be issued until to be
    successful! Disabled by default. */
    do {
        rc = pbackend->modbus_send(ctx, msg, msg_length);
        if (rc == -1) {
            error_print(ctx, NULL);
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) {
                int saved_errno = errno;

                if ((errno == EBADF || errno == ECONNRESET || errno == EPIPE)) {
                    pbackend->modbus_close(ctx);
                    sleep_response_timeout(ctx);
                    pbackend->modbus_connect(ctx);
                }
                else {
                    sleep_response_timeout(ctx);
                    pbackend->modbus_flush(ctx);
                }
                errno = saved_errno;
            }
        }
    } while ((ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) &&
        rc == -1);

    if (rc > 0 && rc != msg_length) {
        errno = EMBBADDATA;
        return -1;
    }

    return rc;
}

void modbus_common::sleep_response_timeout(modbus_t *ctx)
{
    Sleep((ctx->response_timeout.tv_sec * 1000) +
        (ctx->response_timeout.tv_usec / 1000));
}

uint8_t modbus_common::compute_meta_length_after_function(int function, msg_type_t msg_type)
{
    int length;

    if (msg_type == MSG_INDICATION) {
        if (function <= MODBUS_FC_WRITE_SINGLE_REGISTER) {
            length = 4;
        }
        else if (function == MODBUS_FC_WRITE_MULTIPLE_COILS ||
            function == MODBUS_FC_WRITE_MULTIPLE_REGISTERS) {
            length = 5;
        }
        else if (function == MODBUS_FC_MASK_WRITE_REGISTER) {
            length = 6;
        }
        else if (function == MODBUS_FC_WRITE_AND_READ_REGISTERS) {
            length = 9;
        }
        else {
            /* MODBUS_FC_READ_EXCEPTION_STATUS, MODBUS_FC_REPORT_SLAVE_ID */
            length = 0;
        }
    }
    else {
        /* MSG_CONFIRMATION */
        switch (function) {
        case MODBUS_FC_WRITE_SINGLE_COIL:
        case MODBUS_FC_WRITE_SINGLE_REGISTER:
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
            length = 4;
            break;
        case MODBUS_FC_MASK_WRITE_REGISTER:
            length = 6;
            break;
        default:
            length = 1;
        }
    }

    return length;
}

/* Computes the length to read after the meta information (address, count, etc) */
int modbus_common::compute_data_length_after_meta(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type, interface_backend *pbackend)
{
    int function = msg[pbackend->modbus_header_length()];
    int length;

    if (msg_type == MSG_INDICATION) {
        switch (function) {
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
            length = msg[pbackend->modbus_header_length() + 5];
            break;
        case MODBUS_FC_WRITE_AND_READ_REGISTERS:
            length = msg[pbackend->modbus_header_length() + 9];
            break;
        default:
            length = 0;
        }
    }
    else {
        /* MSG_CONFIRMATION */
        if (function <= MODBUS_FC_READ_INPUT_REGISTERS ||
            function == MODBUS_FC_REPORT_SLAVE_ID ||
            function == MODBUS_FC_WRITE_AND_READ_REGISTERS) {
            length = msg[pbackend->modbus_header_length() + 1];
        }
        else {
            length = 0;
        }
    }

    length += pbackend->modbus_checksum_length();

    return length;
}

uint16_t modbus_common::crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

void modbus_common::init_common(modbus_t *ctx)
{
    /* Slave and socket are initialized to -1 */
    ctx->slave = -1;
    ctx->s = -1;

    ctx->debug = FALSE;
    ctx->error_recovery = MODBUS_ERROR_RECOVERY_NONE;

    ctx->response_timeout.tv_sec = 0;
    ctx->response_timeout.tv_usec = _RESPONSE_TIMEOUT;

    ctx->byte_timeout.tv_sec = 0;
    ctx->byte_timeout.tv_usec = _BYTE_TIMEOUT;
}

int modbus_common::tcp_init_win32()
{
    /* Initialise Windows Socket API */
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup() returned error code %d\n",
            (unsigned int)GetLastError());
        errno = EIO;
        return -1;
    }
    return 0;
}

void modbus_common::error_print(modbus_t *ctx, const char *context)
{
    if (ctx->debug) {
        fprintf(stderr, "ERROR %s", get_error_string(errno));
        if (context != NULL) {
            fprintf(stderr, ": %s\n", context);
        }
        else {
            fprintf(stderr, "\n");
        }
    }
}

/*
* Function strlcpy was originally developed by
* Todd C. Miller <Todd.Miller@courtesan.com> to simplify writing secure code.
* See ftp://ftp.openbsd.org/pub/OpenBSD/src/lib/libc/string/strlcpy.3
* for more information.
*
* Thank you Ulrich Drepper... not!
*
* Copy src to string dest of size dest_size.  At most dest_size-1 characters
* will be copied.  Always NUL terminates (unless dest_size == 0).  Returns
* strlen(src); if retval >= dest_size, truncation occurred.
*/
size_t modbus_common::strlcpy(char *dest, const char *src, size_t dest_size)
{
    register char *d = dest;
    register const char *s = src;
    register size_t n = dest_size;

    /* Copy as many bytes as will fit */
    if (n != 0 && --n != 0) {
        do {
            if ((*d++ = *s++) == 0)
                break;
        } while (--n != 0);
    }

    /* Not enough room in dest, add NUL and traverse rest of src */
    if (n == 0) {
        if (dest_size != 0)
            *d = '\0'; /* NUL-terminate dest */
        while (*s++)
            ;
    }

    return (s - src - 1); /* count does not include NUL */
}

/* Write a value to the specified register of the remote device.
Used by write_bit and write_register */
int modbus_common::write_single(modbus_t *ctx, int function, int addr, int value, interface_backend *pbackend)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    req_length = pbackend->modbus_build_request_basis(ctx, function, addr, value, req);

    rc = send_msg(ctx, req, req_length, pbackend);
    if (rc > 0) {
        /* Used by write_bit and write_register */
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION, pbackend);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc, pbackend);
    }

    return rc;
}

/* Reads the data from a remove device and put that data into an array */
int modbus_common::read_registers(modbus_t *ctx, int function, int addr, int nb, uint16_t *dest, interface_backend *pbackend)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_READ_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Too many registers requested (%d > %d)\n",
                nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = pbackend->modbus_build_request_basis(ctx, function, addr, nb, req);

    rc = send_msg(ctx, req, req_length, pbackend);
    if (rc > 0) {
        int offset;
        int i;

        rc = receive_msg(ctx, rsp, MSG_CONFIRMATION, pbackend);
        if (rc == -1)
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc, pbackend);
        if (rc == -1)
            return -1;

        offset = pbackend->modbus_header_length();

        for (i = 0; i < rc; i++) {
            /* shift reg hi_byte to temp OR with lo_byte */
            dest[i] = (rsp[offset + 2 + (i << 1)] << 8) |
                rsp[offset + 3 + (i << 1)];
        }
    }

    return rc;
}

/* Computes the length of the expected response */
unsigned int modbus_common::compute_response_length_from_request(modbus_t *ctx, uint8_t *req, interface_backend *pbackend)
{
    int length;
    const int offset = pbackend->modbus_header_length();

    switch (req[offset]) {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS: {
        /* Header + nb values (code from write_bits) */
        int nb = (req[offset + 3] << 8) | req[offset + 4];
        length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
    }
                                         break;
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
        break;
    case MODBUS_FC_READ_EXCEPTION_STATUS:
        length = 3;
        break;
    case MODBUS_FC_REPORT_SLAVE_ID:
        /* The response is device specific (the header provides the
        length) */
        return MSG_LENGTH_UNDEFINED;
    case MODBUS_FC_MASK_WRITE_REGISTER:
        length = 7;
        break;
    default:
        length = 5;
    }

    return offset + length + pbackend->modbus_checksum_length();
}

int modbus_common::response_io_status(uint8_t *tab_io_status, int address, int nb, uint8_t *rsp, int offset)
{
    int shift = 0;
    /* Instead of byte (not allowed in Win32) */
    int one_byte = 0;
    int i;

    for (i = address; i < address + nb; i++) {
        one_byte |= tab_io_status[i] << shift;
        if (shift == 7) {
            /* Byte is full */
            rsp[offset++] = one_byte;
            one_byte = shift = 0;
        }
        else {
            shift++;
        }
    }

    if (shift != 0)
        rsp[offset++] = one_byte;

    return offset;
}

/* Build the exception response */
int modbus_common::response_exception(modbus_t *ctx, sft_t *sft, int exception_code, uint8_t *rsp, unsigned int to_flush, interface_backend *pbackend, const char* temp, ...)
{
    int rsp_length;

    /* Print debug message */
    if (ctx->debug) {
        va_list ap;

        va_start(ap, temp);
        vfprintf(stderr, temp, ap);
        va_end(ap);
    }

    /* Flush if required */
    if (to_flush) {
        sleep_response_timeout(ctx);
        pbackend->modbus_flush(ctx);
    }

    /* Build exception response */
    sft->function = sft->function + 0x80;
    rsp_length = pbackend->modbus_build_response_basis(sft, rsp);
    rsp[rsp_length++] = exception_code;

    return rsp_length;
}

const char * modbus_common::get_error_string(int errnum)
{
    switch (errnum)
    {
    case EMBXILFUN:
        return "Illegal function";
    case EMBXILADD:
        return "Illegal data address";
    case EMBXILVAL:
        return "Illegal data value";
    case EMBXSFAIL:
        return "Slave device or server failure";
    case EMBXACK:
        return "Acknowledge";
    case EMBXSBUSY:
        return "Slave device or server is busy";
    case EMBXNACK:
        return "Negative acknowledge";
    case EMBXMEMPAR:
        return "Memory parity error";
    case EMBXGPATH:
        return "Gateway path unavailable";
    case EMBXGTAR:
        return "Target device failed to respond";
    case EMBBADCRC:
        return "Invalid CRC";
    case EMBBADDATA:
        return "Invalid data";
    case EMBBADEXC:
        return "Invalid exception code";
    case EMBMDATA:
        return "Too many data";
    case EMBBADSLAVE:
        return "Response not from requested slave";
    default: 
        {
            char buffer[512];
            strerror_s(buffer, errnum);
            return string(buffer).c_str();
        }
    }
}
