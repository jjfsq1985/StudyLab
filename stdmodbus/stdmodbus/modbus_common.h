#pragma once

#include "modbus_def.h"
#include "modbus-private.h"
#include "interface_backend.h"

class modbus_common
{
public:
    modbus_common();
    ~modbus_common();

public:
    static int receive_msg(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type, interface_backend *pbackend);
    static int read_io_status(modbus_t *ctx, int function, int addr, int nb, uint8_t *dest, interface_backend *pbackend);
    static int send_msg(modbus_t *ctx, uint8_t *msg, int msg_length, interface_backend *pbackend);
    static int check_confirmation(modbus_t *ctx, uint8_t *req, uint8_t *rsp, int rsp_length, interface_backend *pbackend);
    static int write_single(modbus_t *ctx, int function, int addr, int value, interface_backend *pbackend);
    static int read_registers(modbus_t *ctx, int function, int addr, int nb, uint16_t *dest, interface_backend *pbackend);
    static int compute_data_length_after_meta(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type, interface_backend *pbackend);
    static unsigned int compute_response_length_from_request(modbus_t *ctx, uint8_t *req, interface_backend *pbackend);

public:
    static int tcp_init_win32();
    static void init_common(modbus_t *ctx);
    static void error_print(modbus_t *ctx, const char *context);
    static const char * get_error_string(int errnum);
    static void sleep_response_timeout(modbus_t *ctx);
    static int response_io_status(uint8_t *tab_io_status, int address, int nb, uint8_t *rsp, int offset);
    static uint8_t compute_meta_length_after_function(int function, msg_type_t msg_type);
    static uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);
    static size_t strlcpy(char *dest, const char *src, size_t dest_size);

public:
    static int response_exception(modbus_t *ctx, sft_t *sft, int exception_code, uint8_t *rsp, unsigned int to_flush, interface_backend *pbackend, const char* temp, ...);

};

