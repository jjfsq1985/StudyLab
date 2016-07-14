/*
* Copyright (c) 2001-2011 Stephane Raimbault <stephane.raimbault@gmail.com>
*
* SPDX-License-Identifier: LGPL-2.1+
*
* This library implements the Modbus protocol.
* http://libmodbus.org/
* jiangjf <jjfsq1985@gmail.com> refactor to c++
*/

#ifndef MODBUS_DEF_H
#define MODBUS_DEF_H

#include <stdint.h>

#if defined(STD_MODBUS_EXPORTS)
#define MODBUS_API __declspec(dllexport)
#else
#define MODBUS_API __declspec(dllimport)
#endif

#ifdef  __cplusplus
#define MODBUS_BEGIN_DECLS  extern "C" {
#define MODBUS_END_DECLS    }
#else
#define MODBUS_BEGIN_DECLS
#define MODBUS_END_DECLS
#endif

MODBUS_BEGIN_DECLS

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

/* Modbus function codes */
#define MODBUS_FC_READ_COILS                0x01
#define MODBUS_FC_READ_DISCRETE_INPUTS      0x02
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_READ_INPUT_REGISTERS      0x04
#define MODBUS_FC_WRITE_SINGLE_COIL         0x05
#define MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
#define MODBUS_FC_READ_EXCEPTION_STATUS     0x07
#define MODBUS_FC_WRITE_MULTIPLE_COILS      0x0F
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10
#define MODBUS_FC_REPORT_SLAVE_ID           0x11
#define MODBUS_FC_MASK_WRITE_REGISTER       0x16
#define MODBUS_FC_WRITE_AND_READ_REGISTERS  0x17

#define MODBUS_BROADCAST_ADDRESS    0

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
* Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
* (chapter 6 section 11 page 29)
* Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
*/
#define MODBUS_MAX_READ_BITS              2000
#define MODBUS_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
* Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
* (chapter 6 section 12 page 31)
* Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
* (chapter 6 section 17 page 38)
* Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
*/
#define MODBUS_MAX_READ_REGISTERS          125
#define MODBUS_MAX_WRITE_REGISTERS         123
#define MODBUS_MAX_WR_WRITE_REGISTERS      121
#define MODBUS_MAX_WR_READ_REGISTERS       125

/* The size of the MODBUS PDU is limited by the size constraint inherited from
* the first MODBUS implementation on Serial Line network (max. RS485 ADU = 256
* bytes). Therefore, MODBUS PDU for serial line communication = 256 - Server
* address (1 byte) - CRC (2 bytes) = 253 bytes.
*/
#define MODBUS_MAX_PDU_LENGTH              253

/* Consequently:
* - RTU MODBUS ADU = 253 bytes + Server address (1 byte) + CRC (2 bytes) = 256
*   bytes.
* - TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes.
* so the maximum of both backend in 260 bytes. This size can used to allocate
* an array of bytes to store responses and it will be compatible with the two
* backends.
*/
#define MODBUS_MAX_ADU_LENGTH              260

/* Max between RTU and TCP max adu length (so TCP) */
#define MAX_MESSAGE_LENGTH 260

/* Random number to avoid errno conflicts */
#define MODBUS_ENOBASE 112345678

/* Protocol exceptions */
enum {
    MODBUS_EXCEPTION_ILLEGAL_FUNCTION = 0x01,
    MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
    MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
    MODBUS_EXCEPTION_ACKNOWLEDGE,
    MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
    MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
    MODBUS_EXCEPTION_MEMORY_PARITY,
    MODBUS_EXCEPTION_NOT_DEFINED,
    MODBUS_EXCEPTION_GATEWAY_PATH,
    MODBUS_EXCEPTION_GATEWAY_TARGET,
    MODBUS_EXCEPTION_MAX
};

#define EMBXILFUN  (MODBUS_ENOBASE + MODBUS_EXCEPTION_ILLEGAL_FUNCTION)
#define EMBXILADD  (MODBUS_ENOBASE + MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS)
#define EMBXILVAL  (MODBUS_ENOBASE + MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE)
#define EMBXSFAIL  (MODBUS_ENOBASE + MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE)
#define EMBXACK    (MODBUS_ENOBASE + MODBUS_EXCEPTION_ACKNOWLEDGE)
#define EMBXSBUSY  (MODBUS_ENOBASE + MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY)
#define EMBXNACK   (MODBUS_ENOBASE + MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE)
#define EMBXMEMPAR (MODBUS_ENOBASE + MODBUS_EXCEPTION_MEMORY_PARITY)
#define EMBXGPATH  (MODBUS_ENOBASE + MODBUS_EXCEPTION_GATEWAY_PATH)
#define EMBXGTAR   (MODBUS_ENOBASE + MODBUS_EXCEPTION_GATEWAY_TARGET)

/* Native asio_modbus error codes */
#define EMBBADCRC  (EMBXGTAR + 1)
#define EMBBADDATA (EMBXGTAR + 2)
#define EMBBADEXC  (EMBXGTAR + 3)
#define EMBUNKEXC  (EMBXGTAR + 4)
#define EMBMDATA   (EMBXGTAR + 5)
#define EMBBADSLAVE (EMBXGTAR + 6)

typedef struct _modbus modbus_t;//_modbus结构体不需要导出

typedef struct  _modbus_mapping{
    int nb_bits;
    int start_bits;
    int nb_input_bits;
    int start_input_bits;
    int nb_input_registers;
    int start_input_registers;
    int nb_registers;
    int start_registers;
    uint8_t *tab_bits;
    uint8_t *tab_input_bits;
    uint16_t *tab_input_registers;
    uint16_t *tab_registers;
} modbus_mapping_t;

typedef enum
{
    MODBUS_ERROR_RECOVERY_NONE = 0,
    MODBUS_ERROR_RECOVERY_LINK = (1 << 1),
    MODBUS_ERROR_RECOVERY_PROTOCOL = (1 << 2)
} modbus_error_recovery_mode;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <winsock2.h>
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
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

#define MODBUS_RTU_RS232 0
#define MODBUS_RTU_RS485 1

#define MODBUS_RTU_RTS_NONE   0
#define MODBUS_RTU_RTS_UP     1
#define MODBUS_RTU_RTS_DOWN   2

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
* RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
*/
#define MODBUS_RTU_MAX_ADU_LENGTH  256

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/**
* UTILS FUNCTIONS
**/
//ModBus 使用Big Endian 格式
#define MODBUS_GET_HIGH_BYTE(data)  (((data) >> 8) & 0xFF)
#define MODBUS_GET_LOW_BYTE(data)   ((data) & 0xFF)
#define MODBUS_GET_INT64_FROM_INT16(tab_int16, index) \
    (((int64_t)tab_int16[(index)    ] << 48) + \
     ((int64_t)tab_int16[(index) + 1] << 32) + \
     ((int64_t)tab_int16[(index) + 2] << 16) + \
      (int64_t)tab_int16[(index) + 3])
#define MODBUS_GET_INT32_FROM_INT16(tab_int16, index) \
    ((tab_int16[(index)] << 16) + \
      tab_int16[(index) + 1])
#define MODBUS_GET_INT16_FROM_INT8(tab_int8, index) \
    ((tab_int8[(index)] << 8) + \
      tab_int8[(index) + 1])
#define MODBUS_SET_INT16_TO_INT8(tab_int8, index, value) \
    do { \
        tab_int8[(index)] = (value) >> 8;  \
        tab_int8[(index) + 1] = (value) & 0xFF; \
        } while (0)
#define MODBUS_SET_INT32_TO_INT16(tab_int16, index, value) \
    do { \
        tab_int16[(index)    ] = (value) >> 16; \
        tab_int16[(index) + 1] = (value); \
        } while (0)
#define MODBUS_SET_INT64_TO_INT16(tab_int16, index, value) \
    do { \
        tab_int16[(index)    ] = (value) >> 48; \
        tab_int16[(index) + 1] = (value) >> 32; \
        tab_int16[(index) + 2] = (value) >> 16; \
        tab_int16[(index) + 3] = (value); \
        } while (0)
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

MODBUS_END_DECLS

#endif
