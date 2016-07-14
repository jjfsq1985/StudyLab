#ifndef STDMODBUS_H
#define STDMODBUS_H

#include "modbus_def.h"

MODBUS_BEGIN_DECLS

MODBUS_API int modbus_set_slave(modbus_t* ctx, int slave);
MODBUS_API int modbus_set_error_recovery(modbus_t *ctx, modbus_error_recovery_mode error_recovery);
MODBUS_API int modbus_set_socket(modbus_t *ctx, int s);
MODBUS_API int modbus_get_socket(modbus_t *ctx);

MODBUS_API int modbus_get_response_timeout(modbus_t *ctx, uint32_t *to_sec, uint32_t *to_usec);
MODBUS_API int modbus_set_response_timeout(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec);

MODBUS_API int modbus_get_byte_timeout(modbus_t *ctx, uint32_t *to_sec, uint32_t *to_usec);
MODBUS_API int modbus_set_byte_timeout(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec);

MODBUS_API int modbus_get_header_length(modbus_t *ctx);

MODBUS_API int modbus_connect(modbus_t *ctx);
MODBUS_API void modbus_close(modbus_t *ctx);

MODBUS_API void modbus_free(modbus_t *ctx);

MODBUS_API int modbus_flush(modbus_t *ctx);
MODBUS_API int modbus_set_debug(modbus_t *ctx, int flag);

MODBUS_API const char *modbus_strerror(int errnum);

MODBUS_API int modbus_read_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest);
MODBUS_API int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest);
MODBUS_API int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);
MODBUS_API int modbus_read_input_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);
MODBUS_API int modbus_write_bit(modbus_t *ctx, int coil_addr, int status);
MODBUS_API int modbus_write_register(modbus_t *ctx, int reg_addr, int value);
MODBUS_API int modbus_write_bits(modbus_t *ctx, int addr, int nb, const uint8_t *data);
MODBUS_API int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *data);
MODBUS_API int modbus_mask_write_register(modbus_t *ctx, int addr, uint16_t and_mask, uint16_t or_mask);
MODBUS_API int modbus_write_and_read_registers(modbus_t *ctx, int write_addr, int write_nb, const uint16_t *src, int read_addr, int read_nb, uint16_t *dest);
MODBUS_API int modbus_report_slave_id(modbus_t *ctx, int max_dest, uint8_t *dest);

MODBUS_API modbus_mapping_t* modbus_mapping_new_start_address(unsigned int start_bits, unsigned int nb_bits, unsigned int start_input_bits, unsigned int nb_input_bits, unsigned int start_registers, unsigned int nb_registers, unsigned int start_input_registers, unsigned int nb_input_registers);

MODBUS_API modbus_mapping_t* modbus_mapping_new(int nb_bits, int nb_input_bits, int nb_registers, int nb_input_registers);
MODBUS_API void modbus_mapping_free(modbus_mapping_t *mb_mapping);

MODBUS_API int modbus_send_raw_request(modbus_t *ctx, uint8_t *raw_req, int raw_req_length);

MODBUS_API int modbus_receive(modbus_t *ctx, uint8_t *req);

MODBUS_API int modbus_receive_confirmation(modbus_t *ctx, uint8_t *rsp);

MODBUS_API int modbus_reply(modbus_t *ctx, const uint8_t *req, int req_length, modbus_mapping_t *mb_mapping);
MODBUS_API int modbus_reply_exception(modbus_t *ctx, const uint8_t *req, unsigned int exception_code);

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/*一般用于IPv4*/
MODBUS_API modbus_t* modbus_new_tcp(const char *ip_address, int port);
MODBUS_API int modbus_tcp_listen(modbus_t *ctx, int nb_connection);
MODBUS_API int modbus_tcp_accept(modbus_t *ctx, int *s);

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/*TCP PI((Protocol Indepedent协议无关))
*可用于IPv6, 提供主机名解析,但要多消耗约1Kb内存
*/
MODBUS_API modbus_t* modbus_new_tcp_pi(const char *node, const char *service);
MODBUS_API int modbus_tcp_pi_listen(modbus_t *ctx, int nb_connection);
MODBUS_API int modbus_tcp_pi_accept(modbus_t *ctx, int *s);

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//RTU  串口 RS232/RS485
MODBUS_API modbus_t* modbus_new_rtu(const char *device, int baud, char parity,
    int data_bit, int stop_bit);
MODBUS_API int modbus_rtu_set_serial_mode(modbus_t *ctx, int mode);
MODBUS_API int modbus_rtu_get_serial_mode(modbus_t *ctx);

MODBUS_API int modbus_rtu_set_rts(modbus_t *ctx, int mode);
MODBUS_API int modbus_rtu_get_rts(modbus_t *ctx);

MODBUS_API int modbus_rtu_set_custom_rts(modbus_t *ctx, void(*set_rts) (modbus_t *ctx, int on));

MODBUS_API int modbus_rtu_set_rts_delay(modbus_t *ctx, int us);
MODBUS_API int modbus_rtu_get_rts_delay(modbus_t *ctx);

MODBUS_END_DECLS

#endif
