#ifndef MODBUS_TCPPI_PRIVATE_H
#define MODBUS_TCPPI_PRIVATE_H

#define _MODBUS_TCP_PI_NODE_LENGTH    1025
#define _MODBUS_TCP_PI_SERVICE_LENGTH   32

typedef struct _modbus_tcp_pi {
    /* Transaction ID */
    uint16_t t_id;
    /* TCP port */
    int port;
    /* Node */
    char node[_MODBUS_TCP_PI_NODE_LENGTH];
    /* Service */
    char service[_MODBUS_TCP_PI_SERVICE_LENGTH];
} modbus_tcp_pi_t;

#endif
