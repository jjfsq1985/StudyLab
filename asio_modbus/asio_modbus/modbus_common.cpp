#include "stdafx.h"
#include "modbus_common.h"

/* 3 steps are used to parse the query */
typedef enum {
    _STEP_FUNCTION,
    _STEP_META,
    _STEP_DATA
} _step_t;

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
    length_to_read = pbackend->header_length + 1;

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
            _error_print(ctx, "select");
            if (ctx->error_recovery & MODBUS_ERROR_RECOVERY_LINK) {
                int saved_errno = errno;

                if (errno == ETIMEDOUT) {
                    sleep_response_timeout(ctx);
                    modbus_flush(ctx);
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
            _error_print(ctx, "read");
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
                    msg[ctx->backend->header_length],
                    msg_type);
                if (length_to_read != 0) {
                    step = _STEP_META;
                    break;
                } /* else switches straight to the next step */
            case _STEP_META:
                length_to_read = compute_data_length_after_meta(
                    ctx, msg, msg_type);
                if ((msg_length + length_to_read) > (int)ctx->backend->max_adu_length) {
                    errno = EMBBADDATA;
                    _error_print(ctx, "too many data");
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
int modbus_common::compute_data_length_after_meta(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type)
{
    int function = msg[ctx->backend->header_length];
    int length;

    if (msg_type == MSG_INDICATION) {
        switch (function) {
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
            length = msg[ctx->backend->header_length + 5];
            break;
        case MODBUS_FC_WRITE_AND_READ_REGISTERS:
            length = msg[ctx->backend->header_length + 9];
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
            length = msg[ctx->backend->header_length + 1];
        }
        else {
            length = 0;
        }
    }

    length += ctx->backend->checksum_length;

    return length;
}