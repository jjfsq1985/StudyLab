/*
* Copyright (c) 2001-2011 Stephane Raimbault <stephane.raimbault@gmail.com>
*
* SPDX-License-Identifier: LGPL-2.1+
*
* This library implements the Modbus protocol.
* http://libmodbus.org/
* jiangjf <jjfsq1985@gmail.com> refactor to c++
*/

#include "stdafx.h"
#include "modbus_def.h"

#include <stdarg.h>
#include <limits.h>
#include <time.h>
#include <string>
using namespace std;

#include "stdmodbus.h"
#include "modbus-private.h"
#include "modbus-rtu-private.h"
#include "modbus-tcp-private.h"
#include "modbus-tcpPI-private.h"
#include "modbus_common.h"
#include "modbus-data.h"

#include "modbus_rtu_backend.h"
#include "modbus_tcp_backend.h"
#include "modbus_tcpPI_backend.h"

const string VERSION_STRING = "0.1.1";

/* Define the slave number */
int modbus_set_slave(modbus_t *ctx, int slave)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->modbus_set_slave(ctx, slave);
}

int modbus_set_error_recovery(modbus_t *ctx, modbus_error_recovery_mode error_recovery)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* The type of modbus_error_recovery_mode is unsigned enum */
    ctx->error_recovery = (uint8_t)error_recovery;
    return 0;
}

int modbus_set_socket(modbus_t *ctx, int s)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    ctx->s = s;
    return 0;
}

int modbus_get_socket(modbus_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->s;
}

/* Get the timeout interval used to wait for a response */
int modbus_get_response_timeout(modbus_t *ctx, uint32_t *to_sec, uint32_t *to_usec)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    *to_sec = ctx->response_timeout.tv_sec;
    *to_usec = ctx->response_timeout.tv_usec;
    return 0;
}

int modbus_set_response_timeout(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec)
{
    if (ctx == NULL ||
        (to_sec == 0 && to_usec == 0) || to_usec > 999999) {
        errno = EINVAL;
        return -1;
    }

    ctx->response_timeout.tv_sec = to_sec;
    ctx->response_timeout.tv_usec = to_usec;
    return 0;
}


/* Get the timeout interval between two consecutive bytes of a message */
int modbus_get_byte_timeout(modbus_t *ctx, uint32_t *to_sec, uint32_t *to_usec)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    *to_sec = ctx->byte_timeout.tv_sec;
    *to_usec = ctx->byte_timeout.tv_usec;
    return 0;
}

int modbus_set_byte_timeout(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec)
{
    /* Byte timeout can be disabled when both values are zero */
    if (ctx == NULL || to_usec > 999999) {
        errno = EINVAL;
        return -1;
    }

    ctx->byte_timeout.tv_sec = to_sec;
    ctx->byte_timeout.tv_usec = to_usec;
    return 0;
}

int modbus_get_header_length(modbus_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->header_length;
}

int modbus_connect(modbus_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->modbus_connect(ctx);
}

void modbus_close(modbus_t *ctx)
{
    if (ctx == NULL)
        return;

    ctx->backend->modbus_close(ctx);
}

void modbus_free(modbus_t *ctx)
{
    if (ctx == NULL)
        return;

    ctx->backend->modbus_free(ctx);
}


int modbus_flush(modbus_t *ctx)
{
    int rc;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    rc = ctx->backend->modbus_flush(ctx);
    if (rc != -1 && ctx->debug) {
        /* Not all backends are able to return the number of bytes flushed */
        printf("Bytes flushed (%d)\n", rc);
    }
    return rc;
}

int modbus_set_debug(modbus_t *ctx, int flag)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    ctx->debug = flag;
    return 0;
}

const char *modbus_strerror(int errnum)
{
    return modbus_common::get_error_string(errnum);
}

/* Reads the boolean status of bits and sets the array elements
in the destination to TRUE or FALSE (single bits). */
int modbus_read_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest)
{
    int rc;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (nb > MODBUS_MAX_READ_BITS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Too many bits requested (%d > %d)\n",
                nb, MODBUS_MAX_READ_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    rc = modbus_common::read_io_status(ctx, MODBUS_FC_READ_COILS, addr, nb, dest,ctx->backend);

    if (rc == -1)
        return -1;
    else
        return nb;
}

/* Same as modbus_read_bits but reads the remote device input table */
int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest)
{
    int rc;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (nb > MODBUS_MAX_READ_BITS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Too many discrete inputs requested (%d > %d)\n",
                nb, MODBUS_MAX_READ_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    rc = modbus_common::read_io_status(ctx, MODBUS_FC_READ_DISCRETE_INPUTS, addr, nb, dest,ctx->backend);

    if (rc == -1)
        return -1;
    else
        return nb;
}

/* Reads the holding registers of remote device and put the data into an
array */
int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest)
{
    int status;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (nb > MODBUS_MAX_READ_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Too many registers requested (%d > %d)\n",
                nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    status = modbus_common::read_registers(ctx, MODBUS_FC_READ_HOLDING_REGISTERS, addr, nb, dest,ctx->backend);
    return status;
}

/* Reads the input registers of remote device and put the data into an array */
int modbus_read_input_registers(modbus_t *ctx, int addr, int nb,
    uint16_t *dest)
{
    int status;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (nb > MODBUS_MAX_READ_REGISTERS) {
        fprintf(stderr,
            "ERROR Too many input registers requested (%d > %d)\n",
            nb, MODBUS_MAX_READ_REGISTERS);
        errno = EMBMDATA;
        return -1;
    }

    status = modbus_common::read_registers(ctx, MODBUS_FC_READ_INPUT_REGISTERS, addr, nb, dest, ctx->backend);

    return status;
}

/* Turns ON or OFF a single bit of the remote device */
int modbus_write_bit(modbus_t *ctx, int addr, int status)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return modbus_common::write_single(ctx, MODBUS_FC_WRITE_SINGLE_COIL, addr, status ? 0xFF00 : 0, ctx->backend);
}

/* Writes a value in one register of the remote device */
int modbus_write_register(modbus_t *ctx, int addr, int value)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return modbus_common::write_single(ctx, MODBUS_FC_WRITE_SINGLE_REGISTER, addr, value,ctx->backend);
}

/* Write the bits of the array in the remote device */
int modbus_write_bits(modbus_t *ctx, int addr, int nb, const uint8_t *src)
{
    int rc;
    int i;
    int byte_count;
    int req_length;
    int bit_check = 0;
    int pos = 0;
    uint8_t req[MAX_MESSAGE_LENGTH];

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (nb > MODBUS_MAX_WRITE_BITS) {
        if (ctx->debug) {
            fprintf(stderr, "ERROR Writing too many bits (%d > %d)\n",
                nb, MODBUS_MAX_WRITE_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->modbus_build_request_basis(ctx,MODBUS_FC_WRITE_MULTIPLE_COILS,addr, nb, req);
    byte_count = (nb / 8) + ((nb % 8) ? 1 : 0);
    req[req_length++] = byte_count;

    for (i = 0; i < byte_count; i++) {
        int bit;

        bit = 0x01;
        req[req_length] = 0;

        while ((bit & 0xFF) && (bit_check++ < nb)) {
            if (src[pos++])
                req[req_length] |= bit;
            else
                req[req_length] &= ~bit;

            bit = bit << 1;
        }
        req_length++;
    }

    rc = modbus_common::send_msg(ctx, req, req_length,ctx->backend);
    if (rc > 0) {
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        rc = modbus_common::receive_msg(ctx, rsp, MSG_CONFIRMATION, ctx->backend);
        if (rc == -1)
            return -1;

        rc = modbus_common::check_confirmation(ctx, req, rsp, rc, ctx->backend);
    }


    return rc;
}

/* Write the values from the array to the registers of the remote device */
int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *src)
{
    int rc;
    int i;
    int req_length;
    int byte_count;
    uint8_t req[MAX_MESSAGE_LENGTH];

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (nb > MODBUS_MAX_WRITE_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Trying to write to too many registers (%d > %d)\n",
                nb, MODBUS_MAX_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->modbus_build_request_basis(ctx,MODBUS_FC_WRITE_MULTIPLE_REGISTERS, addr, nb, req);
    byte_count = nb * 2;
    req[req_length++] = byte_count;

    for (i = 0; i < nb; i++) {
        req[req_length++] = src[i] >> 8;
        req[req_length++] = src[i] & 0x00FF;
    }

    rc = modbus_common::send_msg(ctx, req, req_length,ctx->backend);
    if (rc > 0) {
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        rc = modbus_common::receive_msg(ctx, rsp, MSG_CONFIRMATION,ctx->backend);
        if (rc == -1)
            return -1;

        rc = modbus_common::check_confirmation(ctx, req, rsp, rc, ctx->backend);
    }

    return rc;
}

int modbus_mask_write_register(modbus_t *ctx, int addr, uint16_t and_mask, uint16_t or_mask)
{
    int rc;
    int req_length;
    /* The request length can not exceed _MIN_REQ_LENGTH - 2 and 4 bytes to
    * store the masks. The ugly substraction is there to remove the 'nb' value
    * (2 bytes) which is not used. */
    uint8_t req[_MIN_REQ_LENGTH + 2];

    req_length = ctx->backend->modbus_build_request_basis(ctx, MODBUS_FC_MASK_WRITE_REGISTER, addr, 0, req);

    /* HACKISH, count is not used */
    req_length -= 2;

    req[req_length++] = and_mask >> 8;
    req[req_length++] = and_mask & 0x00ff;
    req[req_length++] = or_mask >> 8;
    req[req_length++] = or_mask & 0x00ff;

    rc = modbus_common::send_msg(ctx, req, req_length,ctx->backend);
    if (rc > 0) {
        /* Used by write_bit and write_register */
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        rc = modbus_common::receive_msg(ctx, rsp, MSG_CONFIRMATION, ctx->backend);
        if (rc == -1)
            return -1;

        rc = modbus_common::check_confirmation(ctx, req, rsp, rc, ctx->backend);
    }

    return rc;
}

/* Write multiple registers from src array to remote device and read multiple
registers from remote device to dest array. */
int modbus_write_and_read_registers(modbus_t *ctx, int write_addr, int write_nb, const uint16_t *src, int read_addr, int read_nb, uint16_t *dest)
{
    int rc;
    int req_length;
    int i;
    int byte_count;
    uint8_t req[MAX_MESSAGE_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (write_nb > MODBUS_MAX_WR_WRITE_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Too many registers to write (%d > %d)\n",
                write_nb, MODBUS_MAX_WR_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    if (read_nb > MODBUS_MAX_WR_READ_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                "ERROR Too many registers requested (%d > %d)\n",
                read_nb, MODBUS_MAX_WR_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }
    req_length = ctx->backend->modbus_build_request_basis(ctx, MODBUS_FC_WRITE_AND_READ_REGISTERS, read_addr, read_nb, req);

    req[req_length++] = write_addr >> 8;
    req[req_length++] = write_addr & 0x00ff;
    req[req_length++] = write_nb >> 8;
    req[req_length++] = write_nb & 0x00ff;
    byte_count = write_nb * 2;
    req[req_length++] = byte_count;

    for (i = 0; i < write_nb; i++) {
        req[req_length++] = src[i] >> 8;
        req[req_length++] = src[i] & 0x00FF;
    }

    rc = modbus_common::send_msg(ctx, req, req_length,ctx->backend);
    if (rc > 0) {
        int offset;

        rc = modbus_common::receive_msg(ctx, rsp, MSG_CONFIRMATION,ctx->backend);
        if (rc == -1)
            return -1;

        rc = modbus_common::check_confirmation(ctx, req, rsp, rc, ctx->backend);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length;
        for (i = 0; i < rc; i++) {
            /* shift reg hi_byte to temp OR with lo_byte */
            dest[i] = (rsp[offset + 2 + (i << 1)] << 8) |
                rsp[offset + 3 + (i << 1)];
        }
    }

    return rc;
}


/* Send a request to get the slave ID of the device (only available in serial
communication). */
int modbus_report_slave_id(modbus_t *ctx, int max_dest, uint8_t *dest)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];

    if (ctx == NULL || max_dest <= 0) {
        errno = EINVAL;
        return -1;
    }

    req_length = ctx->backend->modbus_build_request_basis(ctx, MODBUS_FC_REPORT_SLAVE_ID, 0, 0, req);

    /* HACKISH, addr and count are not used */
    req_length -= 4;

    rc = modbus_common::send_msg(ctx, req, req_length,ctx->backend);
    if (rc > 0) {
        int i;
        int offset;
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        rc = modbus_common::receive_msg(ctx, rsp, MSG_CONFIRMATION,ctx->backend);
        if (rc == -1)
            return -1;

        rc = modbus_common::check_confirmation(ctx, req, rsp, rc, ctx->backend);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length + 2;

        /* Byte count, slave id, run indicator status and
        additional data. Truncate copy to max_dest. */
        for (i = 0; i < rc && i < max_dest; i++) {
            dest[i] = rsp[offset + i];
        }
    }

    return rc;
}

/* Allocates 4 arrays to store bits, input bits, registers and inputs
registers. The pointers are stored in modbus_mapping structure.

The modbus_mapping_new_ranges() function shall return the new allocated
structure if successful. Otherwise it shall return NULL and set errno to
ENOMEM. */
modbus_mapping_t* modbus_mapping_new_start_address(
    unsigned int start_bits, unsigned int nb_bits,
    unsigned int start_input_bits, unsigned int nb_input_bits,
    unsigned int start_registers, unsigned int nb_registers,
    unsigned int start_input_registers, unsigned int nb_input_registers)
{
    modbus_mapping_t *mb_mapping;

    mb_mapping = (modbus_mapping_t *)malloc(sizeof(modbus_mapping_t));
    if (mb_mapping == NULL) {
        return NULL;
    }

    /* 0X */
    mb_mapping->nb_bits = nb_bits;
    mb_mapping->start_bits = start_bits;
    if (nb_bits == 0) {
        mb_mapping->tab_bits = NULL;
    }
    else {
        /* Negative number raises a POSIX error */
        mb_mapping->tab_bits = (uint8_t *)malloc(nb_bits * sizeof(uint8_t));
        if (mb_mapping->tab_bits == NULL) {
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_bits, 0, nb_bits * sizeof(uint8_t));
    }

    /* 1X */
    mb_mapping->nb_input_bits = nb_input_bits;
    mb_mapping->start_input_bits = start_input_bits;
    if (nb_input_bits == 0) {
        mb_mapping->tab_input_bits = NULL;
    }
    else {
        mb_mapping->tab_input_bits = (uint8_t *)malloc(nb_input_bits * sizeof(uint8_t));
        if (mb_mapping->tab_input_bits == NULL) {
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_input_bits, 0, nb_input_bits * sizeof(uint8_t));
    }

    /* 4X */
    mb_mapping->nb_registers = nb_registers;
    mb_mapping->start_registers = start_registers;
    if (nb_registers == 0) {
        mb_mapping->tab_registers = NULL;
    }
    else {
        mb_mapping->tab_registers = (uint16_t *)malloc(nb_registers * sizeof(uint16_t));
        if (mb_mapping->tab_registers == NULL) {
            free(mb_mapping->tab_input_bits);
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_registers, 0, nb_registers * sizeof(uint16_t));
    }

    /* 3X */
    mb_mapping->nb_input_registers = nb_input_registers;
    mb_mapping->start_input_registers = start_input_registers;
    if (nb_input_registers == 0) {
        mb_mapping->tab_input_registers = NULL;
    }
    else {
        mb_mapping->tab_input_registers = (uint16_t *)malloc(nb_input_registers * sizeof(uint16_t));
        if (mb_mapping->tab_input_registers == NULL) {
            free(mb_mapping->tab_registers);
            free(mb_mapping->tab_input_bits);
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_input_registers, 0,
            nb_input_registers * sizeof(uint16_t));
    }

    return mb_mapping;
}

modbus_mapping_t* modbus_mapping_new(int nb_bits, int nb_input_bits,
    int nb_registers, int nb_input_registers)
{
    return modbus_mapping_new_start_address( 0, nb_bits, 0, nb_input_bits, 0, nb_registers, 0, nb_input_registers);
}

/* Frees the 4 arrays */
void modbus_mapping_free(modbus_mapping_t *mb_mapping)
{
    if (mb_mapping == NULL) {
        return;
    }

    free(mb_mapping->tab_input_registers);
    free(mb_mapping->tab_registers);
    free(mb_mapping->tab_input_bits);
    free(mb_mapping->tab_bits);
    free(mb_mapping);
}

int modbus_send_raw_request(modbus_t *ctx, uint8_t *raw_req, int raw_req_length)
{
    sft_t sft;
    uint8_t req[MAX_MESSAGE_LENGTH];
    int req_length;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (raw_req_length < 2 || raw_req_length >(MODBUS_MAX_PDU_LENGTH + 1)) {
        /* The raw request must contain function and slave at least and
        must not be longer than the maximum pdu length plus the slave
        address. */
        errno = EINVAL;
        return -1;
    }

    sft.slave = raw_req[0];
    sft.function = raw_req[1];
    /* The t_id is left to zero */
    sft.t_id = 0;
    /* This response function only set the header so it's convenient here */
    req_length = ctx->backend->modbus_build_response_basis(&sft, req);

    if (raw_req_length > 2) {
        /* Copy data after function code */
        memcpy(req + req_length, raw_req + 2, raw_req_length - 2);
        req_length += raw_req_length - 2;
    }

    return modbus_common::send_msg(ctx, req, req_length,ctx->backend);
}

/* Receive the request from a modbus master */
int modbus_receive(modbus_t *ctx, uint8_t *req)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->modbus_receive(ctx, req);
}

/* Receives the confirmation.

The function shall store the read response in rsp and return the number of
values (bits or words). Otherwise, its shall return -1 and errno is set.

The function doesn't check the confirmation is the expected response to the
initial request.
*/
int modbus_receive_confirmation(modbus_t *ctx, uint8_t *rsp)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return modbus_common::receive_msg(ctx, rsp, MSG_CONFIRMATION,ctx->backend);
}

/* Send a response to the received request.
Analyses the request and constructs a response.

If an error occurs, this function construct the response
accordingly.
*/
int modbus_reply(modbus_t *ctx, const uint8_t *req, int req_length, modbus_mapping_t *mb_mapping)
{
    int offset;
    int slave;
    int function;
    uint16_t address;
    uint8_t rsp[MAX_MESSAGE_LENGTH];
    int rsp_length = 0;
    sft_t sft;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    offset = ctx->backend->header_length;
    slave = req[offset - 1];
    function = req[offset];
    address = (req[offset + 1] << 8) + req[offset + 2];

    sft.slave = slave;
    sft.function = function;
    sft.t_id = ctx->backend->modbus_prepare_response_tid(req, &req_length);

    /* Data are flushed on illegal number of values errors. */
    switch (function) {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS: {
        unsigned int is_input = (function == MODBUS_FC_READ_DISCRETE_INPUTS);
        int start_bits = is_input ? mb_mapping->start_input_bits : mb_mapping->start_bits;
        int nb_bits = is_input ? mb_mapping->nb_input_bits : mb_mapping->nb_bits;
        uint8_t *tab_bits = is_input ? mb_mapping->tab_input_bits : mb_mapping->tab_bits;
        const char * const name = is_input ? "read_input_bits" : "read_bits";
        int nb = (req[offset + 3] << 8) + req[offset + 4];
        /* The mapping can be shifted to reduce memory consumption and it
        doesn't always start at address zero. */
        int mapping_address = address - start_bits;

        if (nb < 1 || MODBUS_MAX_READ_BITS < nb) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp, TRUE,
                ctx->backend,
                "Illegal nb of values %d in %s (max %d)\n",
                nb, name, MODBUS_MAX_READ_BITS);
        }
        else if (mapping_address < 0 || (mapping_address + nb) > nb_bits) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in %s\n",
                mapping_address < 0 ? address : address + nb, name);
        }
        else {
            rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);
            rsp[rsp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
            rsp_length = modbus_common::response_io_status(tab_bits, mapping_address, nb, rsp, rsp_length);
        }
    }
         break;
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS: {
        unsigned int is_input = (function == MODBUS_FC_READ_INPUT_REGISTERS);
        int start_registers = is_input ? mb_mapping->start_input_registers : mb_mapping->start_registers;
        int nb_registers = is_input ? mb_mapping->nb_input_registers : mb_mapping->nb_registers;
        uint16_t *tab_registers = is_input ? mb_mapping->tab_input_registers : mb_mapping->tab_registers;
        const char * const name = is_input ? "read_input_registers" : "read_registers";
        int nb = (req[offset + 3] << 8) + req[offset + 4];
        /* The mapping can be shifted to reduce memory consumption and it
        doesn't always start at address zero. */
        int mapping_address = address - start_registers;

        if (nb < 1 || MODBUS_MAX_READ_REGISTERS < nb) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp, TRUE,
                ctx->backend,
                "Illegal nb of values %d in %s (max %d)\n",
                nb, name, MODBUS_MAX_READ_REGISTERS);
        }
        else if (mapping_address < 0 || (mapping_address + nb) > nb_registers) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in %s\n",
                mapping_address < 0 ? address : address + nb, name);
        }
        else {
            int i;

            rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);
            rsp[rsp_length++] = nb << 1;
            for (i = mapping_address; i < mapping_address + nb; i++) {
                rsp[rsp_length++] = tab_registers[i] >> 8;
                rsp[rsp_length++] = tab_registers[i] & 0xFF;
            }
        }
    }
        break;
    case MODBUS_FC_WRITE_SINGLE_COIL: {
        int mapping_address = address - mb_mapping->start_bits;

        if (mapping_address < 0 || mapping_address >= mb_mapping->nb_bits) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in write_bit\n",
                address);
        }
        else {
            int data = (req[offset + 3] << 8) + req[offset + 4];

            if (data == 0xFF00 || data == 0x0) {
                mb_mapping->tab_bits[mapping_address] = data ? ON : OFF;
                memcpy(rsp, req, req_length);
                rsp_length = req_length;
            }
            else {
                rsp_length = modbus_common::response_exception(
                    ctx, &sft,
                    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp, FALSE,
                    ctx->backend,
                    "Illegal data value 0x%0X in write_bit request at address %0X\n",
                    data, address);
            }
        }
    }
        break;
    case MODBUS_FC_WRITE_SINGLE_REGISTER: {
        int mapping_address = address - mb_mapping->start_registers;

        if (mapping_address < 0 || mapping_address >= mb_mapping->nb_registers) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in write_register\n",
                address);
        }
        else {
            int data = (req[offset + 3] << 8) + req[offset + 4];

            mb_mapping->tab_registers[mapping_address] = data;
            memcpy(rsp, req, req_length);
            rsp_length = req_length;
        }
    }
                                          break;
    case MODBUS_FC_WRITE_MULTIPLE_COILS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];
        int mapping_address = address - mb_mapping->start_bits;

        if (nb < 1 || MODBUS_MAX_WRITE_BITS < nb) {
            /* May be the indication has been truncated on reading because of
            * invalid address (eg. nb is 0 but the request contains values to
            * write) so it's necessary to flush. */
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp, TRUE,
                ctx->backend,
                "Illegal number of values %d in write_bits (max %d)\n",
                nb, MODBUS_MAX_WRITE_BITS);
        }
        else if (mapping_address < 0 ||
            (mapping_address + nb) > mb_mapping->nb_bits) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in write_bits\n",
                mapping_address < 0 ? address : address + nb);
        }
        else {
            /* 6 = byte count */
            modbus_data::modbus_set_bits_from_bytes(mb_mapping->tab_bits, mapping_address, nb, &req[offset + 6]);

            rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);
            /* 4 to copy the bit address (2) and the quantity of bits */
            memcpy(rsp + rsp_length, req + rsp_length, 4);
            rsp_length += 4;
        }
    }
                                         break;
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];
        int mapping_address = address - mb_mapping->start_registers;

        if (nb < 1 || MODBUS_MAX_WRITE_REGISTERS < nb) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp, TRUE,
                ctx->backend,
                "Illegal number of values %d in write_registers (max %d)\n",
                nb, MODBUS_MAX_WRITE_REGISTERS);
        }
        else if (mapping_address < 0 ||
            (mapping_address + nb) > mb_mapping->nb_registers) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in write_registers\n",
                mapping_address < 0 ? address : address + nb);
        }
        else {
            int i, j;
            for (i = mapping_address, j = 6; i < mapping_address + nb; i++, j += 2) {
                /* 6 and 7 = first value */
                mb_mapping->tab_registers[i] =
                    (req[offset + j] << 8) + req[offset + j + 1];
            }

            rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);
            /* 4 to copy the address (2) and the no. of registers */
            memcpy(rsp + rsp_length, req + rsp_length, 4);
            rsp_length += 4;
        }
    }
                                             break;
    case MODBUS_FC_REPORT_SLAVE_ID: {
        int str_len;
        int byte_count_pos;

        rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);
        /* Skip byte count for now */
        byte_count_pos = rsp_length++;
        rsp[rsp_length++] = _REPORT_SLAVE_ID;
        /* Run indicator status to ON */
        rsp[rsp_length++] = 0xFF;
        /* SMB + length of LIBMODBUS_VERSION_STRING */
        str_len = 3 + strlen(VERSION_STRING.c_str());
        string strVerInfo = "SMB" + VERSION_STRING;
        memcpy(rsp + rsp_length, strVerInfo.c_str(), str_len);
        rsp_length += str_len;
        rsp[byte_count_pos] = rsp_length - byte_count_pos - 1;
    }
                                    break;
    case MODBUS_FC_READ_EXCEPTION_STATUS:
        if (ctx->debug) {
            fprintf(stderr, "FIXME Not implemented\n");
        }
        errno = ENOPROTOOPT;
        return -1;
        break;
    case MODBUS_FC_MASK_WRITE_REGISTER: {
        int mapping_address = address - mb_mapping->start_registers;

        if (mapping_address < 0 || mapping_address >= mb_mapping->nb_registers) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data address 0x%0X in write_register\n",
                address);
        }
        else {
            uint16_t data = mb_mapping->tab_registers[mapping_address];
            uint16_t and = (req[offset + 3] << 8) + req[offset + 4];
            uint16_t or = (req[offset + 5] << 8) + req[offset + 6];

            data = (data & and) | (or & (~and));
            mb_mapping->tab_registers[mapping_address] = data;
            memcpy(rsp, req, req_length);
            rsp_length = req_length;
        }
    }
        break;
    case MODBUS_FC_WRITE_AND_READ_REGISTERS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];
        uint16_t address_write = (req[offset + 5] << 8) + req[offset + 6];
        int nb_write = (req[offset + 7] << 8) + req[offset + 8];
        int nb_write_bytes = req[offset + 9];
        int mapping_address = address - mb_mapping->start_registers;
        int mapping_address_write = address_write - mb_mapping->start_registers;

        if (nb_write < 1 || MODBUS_MAX_WR_WRITE_REGISTERS < nb_write ||
            nb < 1 || MODBUS_MAX_WR_READ_REGISTERS < nb ||
            nb_write_bytes != nb_write * 2) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp, TRUE,
                ctx->backend,
                "Illegal nb of values (W%d, R%d) in write_and_read_registers (max W%d, R%d)\n",
                nb_write, nb, MODBUS_MAX_WR_WRITE_REGISTERS, MODBUS_MAX_WR_READ_REGISTERS);
        }
        else if (mapping_address < 0 ||
            (mapping_address + nb) > mb_mapping->nb_registers ||
            mapping_address < 0 ||
            (mapping_address_write + nb_write) > mb_mapping->nb_registers) {
            rsp_length = modbus_common::response_exception(
                ctx, &sft, 
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp, FALSE,
                ctx->backend,
                "Illegal data read address 0x%0X or write address 0x%0X write_and_read_registers\n",
                mapping_address < 0 ? address : address + nb,
                mapping_address_write < 0 ? address_write : address_write + nb_write);
        }
        else {
            int i, j;
            rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);
            rsp[rsp_length++] = nb << 1;

            /* Write first.
            10 and 11 are the offset of the first values to write */
            for (i = mapping_address_write, j = 10;
                i < mapping_address_write + nb_write; i++, j += 2) {
                mb_mapping->tab_registers[i] =
                    (req[offset + j] << 8) + req[offset + j + 1];
            }

            /* and read the data for the response */
            for (i = mapping_address; i < mapping_address + nb; i++) {
                rsp[rsp_length++] = mb_mapping->tab_registers[i] >> 8;
                rsp[rsp_length++] = mb_mapping->tab_registers[i] & 0xFF;
            }
        }
    }
        break;
    default:
        rsp_length = modbus_common::response_exception(
            ctx, &sft, 
            MODBUS_EXCEPTION_ILLEGAL_FUNCTION, rsp, TRUE,
            ctx->backend,
            "Unknown Modbus function code: 0x%0X\n", function);
        break;
    }

    /* Suppress any responses when the request was a broadcast */
    return (slave == MODBUS_BROADCAST_ADDRESS) ? 0 : modbus_common::send_msg(ctx, rsp, rsp_length,ctx->backend);
}

int modbus_reply_exception(modbus_t *ctx, const uint8_t *req,
    unsigned int exception_code)
{
    int offset;
    int slave;
    int function;
    uint8_t rsp[MAX_MESSAGE_LENGTH];
    int rsp_length;
    int dummy_length = 99;
    sft_t sft;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    offset = ctx->backend->header_length;
    slave = req[offset - 1];
    function = req[offset];

    sft.slave = slave;
    sft.function = function + 0x80;;
    sft.t_id = ctx->backend->modbus_prepare_response_tid(req, &dummy_length);
    rsp_length = ctx->backend->modbus_build_response_basis(&sft, rsp);

    /* Positive exception code */
    if (exception_code < MODBUS_EXCEPTION_MAX) {
        rsp[rsp_length++] = exception_code;
        return modbus_common::send_msg(ctx, rsp, rsp_length,ctx->backend);
    }
    else {
        errno = EINVAL;
        return -1;
    }
}


/* Listens for any request from one or many modbus masters in TCP */
int modbus_tcp_listen(modbus_t *ctx, int nb_connection)
{
    int new_s;
    int enable;
    struct sockaddr_in addr;
    modbus_tcp_t *ctx_tcp;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    ctx_tcp = (modbus_tcp_t *)ctx->backend_data;

    if (modbus_common::tcp_init_win32() == -1) {
        return -1;
    }

    new_s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (new_s == -1) {
        return -1;
    }

    enable = 1;
    if (setsockopt(new_s, SOL_SOCKET, SO_REUSEADDR,
        (char *)&enable, sizeof(enable)) == -1) {
        closesocket(new_s);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    /* If the modbus port is < to 1024, we need the setuid root. */
    addr.sin_port = htons(ctx_tcp->port);
    if (ctx_tcp->ip[0] == '0') {
        /* Listen any addresses */
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
        /* Listen only specified IP address */
        inet_pton(AF_INET, ctx_tcp->ip, (void*)&addr.sin_addr);
    }
    if (bind(new_s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        closesocket(new_s);
        return -1;
    }

    if (listen(new_s, nb_connection) == -1) {
        closesocket(new_s);
        return -1;
    }

    return new_s;
}

int modbus_tcp_pi_listen(modbus_t *ctx, int nb_connection)
{
    int rc;
    struct addrinfo *ai_list;
    struct addrinfo *ai_ptr;
    struct addrinfo ai_hints;
    const char *node;
    const char *service;
    int new_s;
    modbus_tcp_pi_t *ctx_tcp_pi;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    ctx_tcp_pi = (modbus_tcp_pi_t *)ctx->backend_data;

    if (modbus_common::tcp_init_win32() == -1) {
        return -1;
    }

    if (ctx_tcp_pi->node[0] == 0) {
        node = NULL; /* == any */
    }
    else {
        node = ctx_tcp_pi->node;
    }

    if (ctx_tcp_pi->service[0] == 0) {
        service = "502";
    }
    else {
        service = ctx_tcp_pi->service;
    }

    memset(&ai_hints, 0, sizeof(ai_hints));
    /* If node is not NULL, than the AI_PASSIVE flag is ignored. */
    ai_hints.ai_flags |= AI_PASSIVE;
#ifdef AI_ADDRCONFIG
    ai_hints.ai_flags |= AI_ADDRCONFIG;
#endif
    ai_hints.ai_family = AF_UNSPEC;
    ai_hints.ai_socktype = SOCK_STREAM;
    ai_hints.ai_addr = NULL;
    ai_hints.ai_canonname = NULL;
    ai_hints.ai_next = NULL;

    ai_list = NULL;
    rc = getaddrinfo(node, service, &ai_hints, &ai_list);
    if (rc != 0) {
        if (ctx->debug) {
            fprintf(stderr, "Error returned by getaddrinfo: %s\n", gai_strerror(rc));
        }
        errno = ECONNREFUSED;
        return -1;
    }

    new_s = -1;
    for (ai_ptr = ai_list; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
        int s;

        s = socket(ai_ptr->ai_family, ai_ptr->ai_socktype,
            ai_ptr->ai_protocol);
        if (s < 0) {
            if (ctx->debug) {
                perror("socket");
            }
            continue;
        }
        else {
            int enable = 1;
            rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                (const char *)&enable, sizeof(enable));
            if (rc != 0) {
                closesocket(s);
                if (ctx->debug) {
                    perror("setsockopt");
                }
                continue;
            }
        }

        rc = bind(s, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
        if (rc != 0) {
            closesocket(s);
            if (ctx->debug) {
                perror("bind");
            }
            continue;
        }

        rc = listen(s, nb_connection);
        if (rc != 0) {
            closesocket(s);
            if (ctx->debug) {
                perror("listen");
            }
            continue;
        }

        new_s = s;
        break;
    }
    freeaddrinfo(ai_list);

    if (new_s < 0) {
        return -1;
    }

    return new_s;
}

int modbus_tcp_accept(modbus_t *ctx, int *s)
{
    struct sockaddr_in addr;
    socklen_t addrlen;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    addrlen = sizeof(addr);
    ctx->s = accept(*s, (struct sockaddr *)&addr, &addrlen);

    if (ctx->s == -1) {
        closesocket(*s);
        *s = -1;
        return -1;
    }

    if (ctx->debug) {
        char cIPParse[16];
        inet_ntop(AF_INET, (void *)&addr.sin_addr, cIPParse, 16);
        printf("The client connection from %s is accepted\n",
            cIPParse);
    }

    return ctx->s;
}

int modbus_tcp_pi_accept(modbus_t *ctx, int *s)
{
    struct sockaddr_storage addr;
    socklen_t addrlen;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    addrlen = sizeof(addr);
    ctx->s = accept(*s, (struct sockaddr *)&addr, &addrlen);
    if (ctx->s == -1) {
        closesocket(*s);
        *s = -1;
    }

    if (ctx->debug) {
        printf("The client connection is accepted.\n");
    }

    return ctx->s;
}

modbus_t* modbus_new_tcp(const char *ip, int port)
{
    modbus_t *ctx;
    modbus_tcp_t *ctx_tcp;
    size_t dest_size;
    size_t ret_size;

    ctx = (modbus_t *)malloc(sizeof(modbus_t));
    modbus_common::init_common(ctx);    

    /* Could be changed after to reach a remote serial Modbus device */
    ctx->slave = MODBUS_TCP_SLAVE;

    ctx->backend = modbus_tcp_backend::GetInstance();

    ctx->backend_data = (modbus_tcp_t *)malloc(sizeof(modbus_tcp_t));
    ctx_tcp = (modbus_tcp_t *)ctx->backend_data;

    if (ip != NULL) {
        dest_size = sizeof(char) * 16;
        ret_size = modbus_common::strlcpy(ctx_tcp->ip, ip, dest_size);
        if (ret_size == 0) {
            fprintf(stderr, "The IP string is empty\n");
            modbus_free(ctx);
            errno = EINVAL;
            return NULL;
        }

        if (ret_size >= dest_size) {
            fprintf(stderr, "The IP string has been truncated\n");
            modbus_free(ctx);
            errno = EINVAL;
            return NULL;
        }
    }
    else {
        ctx_tcp->ip[0] = '0';
    }
    ctx_tcp->port = port;
    ctx_tcp->t_id = 0;

    return ctx;
}


modbus_t* modbus_new_tcp_pi(const char *node, const char *service)
{
    modbus_t *ctx;
    modbus_tcp_pi_t *ctx_tcp_pi;
    size_t dest_size;
    size_t ret_size;

    ctx = (modbus_t *)malloc(sizeof(modbus_t));
    modbus_common::init_common(ctx);

    /* Could be changed after to reach a remote serial Modbus device */
    ctx->slave = MODBUS_TCP_SLAVE;

    ctx->backend = modbus_tcpPI_backend::GetInstance();

    ctx->backend_data = (modbus_tcp_pi_t *)malloc(sizeof(modbus_tcp_pi_t));
    ctx_tcp_pi = (modbus_tcp_pi_t *)ctx->backend_data;

    if (node == NULL) {
        /* The node argument can be empty to indicate any hosts */
        ctx_tcp_pi->node[0] = 0;
    }
    else {
        dest_size = sizeof(char) * _MODBUS_TCP_PI_NODE_LENGTH;
        ret_size = modbus_common::strlcpy(ctx_tcp_pi->node, node, dest_size);
        if (ret_size == 0) {
            fprintf(stderr, "The node string is empty\n");
            modbus_free(ctx);
            errno = EINVAL;
            return NULL;
        }

        if (ret_size >= dest_size) {
            fprintf(stderr, "The node string has been truncated\n");
            modbus_free(ctx);
            errno = EINVAL;
            return NULL;
        }
    }

    if (service != NULL) {
        dest_size = sizeof(char) * _MODBUS_TCP_PI_SERVICE_LENGTH;
        ret_size = modbus_common::strlcpy(ctx_tcp_pi->service, service, dest_size);
    }
    else {
        /* Empty service is not allowed, error catched below. */
        ret_size = 0;
    }

    if (ret_size == 0) {
        fprintf(stderr, "The service string is empty\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    if (ret_size >= dest_size) {
        fprintf(stderr, "The service string has been truncated\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    ctx_tcp_pi->t_id = 0;

    return ctx;
}

int modbus_rtu_set_serial_mode(modbus_t *ctx, int mode)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }

    /* Wrong backend and invalid mode specified */
    errno = EINVAL;
    return -1;
}

int modbus_rtu_get_serial_mode(modbus_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }
    else {
        errno = EINVAL;
        return -1;
    }
}

int modbus_rtu_get_rts(modbus_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }
    else {
        errno = EINVAL;
        return -1;
    }
}

int modbus_rtu_set_rts(modbus_t *ctx, int mode)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }
    /* Wrong backend or invalid mode specified */
    errno = EINVAL;
    return -1;
}

int modbus_rtu_set_custom_rts(modbus_t *ctx, void(*set_rts) (modbus_t *ctx, int on))
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }
    else {
        errno = EINVAL;
        return -1;
    }
}

int modbus_rtu_get_rts_delay(modbus_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }
    else {
        errno = EINVAL;
        return -1;
    }
}

int modbus_rtu_set_rts_delay(modbus_t *ctx, int us)
{
    if (ctx == NULL || us < 0) {
        errno = EINVAL;
        return -1;
    }

    if (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU) {
        if (ctx->debug) {
            fprintf(stderr, "This function isn't supported on your platform\n");
        }
        errno = ENOTSUP;
        return -1;
    }
    else {
        errno = EINVAL;
        return -1;
    }
}

modbus_t* modbus_new_rtu(const char *device,
    int baud, char parity, int data_bit,
    int stop_bit)
{
    modbus_t *ctx;
    modbus_rtu_t *ctx_rtu;

    /* Check device argument */
    if (device == NULL || *device == 0) {
        fprintf(stderr, "The device string is empty\n");
        errno = EINVAL;
        return NULL;
    }

    /* Check baud argument */
    if (baud == 0) {
        fprintf(stderr, "The baud rate value must not be zero\n");
        errno = EINVAL;
        return NULL;
    }

    ctx = (modbus_t *)malloc(sizeof(modbus_t));
    modbus_common::init_common(ctx);
    ctx->backend = modbus_rtu_backend::GetInstance();
    ctx->backend_data = (modbus_rtu_t *)malloc(sizeof(modbus_rtu_t));
    ctx_rtu = (modbus_rtu_t *)ctx->backend_data;
    ctx_rtu->device = NULL;

    /* Device name and \0 */
    ctx_rtu->device = (char *)malloc((strlen(device) + 1) * sizeof(char));
    strcpy_s(ctx_rtu->device, strlen(device), device);

    ctx_rtu->baud = baud;
    if (parity == 'N' || parity == 'E' || parity == 'O') {
        ctx_rtu->parity = parity;
    }
    else {
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }
    ctx_rtu->data_bit = data_bit;
    ctx_rtu->stop_bit = stop_bit;
    ctx_rtu->confirmation_to_ignore = FALSE;

    return ctx;
}
