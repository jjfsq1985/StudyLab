#pragma once
#include "asio_modbus.h"
#include "modbus-private.h"
#include "interface_backend.h"

class modbus_common
{
public:
    modbus_common();
    ~modbus_common();

public:
    static int receive_msg(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type, interface_backend *pbackend);
    static void sleep_response_timeout(modbus_t *ctx);
    static uint8_t compute_meta_length_after_function(int function, msg_type_t msg_type);
    static int compute_data_length_after_meta(modbus_t *ctx, uint8_t *msg, msg_type_t msg_type);
};

