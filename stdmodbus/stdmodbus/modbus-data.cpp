#include "stdafx.h"
#include <stdint.h>
#include "modbus-data.h"

#include <winsock2.h>//for ntohl 

/* Sets many bits from a single byte value (all 8 bits of the byte value are
   set) */
void modbus_data::modbus_set_bits_from_byte(uint8_t *dest, int idx, const uint8_t value)
{
    int i;

    for (i=0; i < 8; i++) {
        dest[idx+i] = (value & (1 << i)) ? 1 : 0;
    }
}

/* Sets many bits from a table of bytes (only the bits between idx and
   idx + nb_bits are set) */
void modbus_data::modbus_set_bits_from_bytes(uint8_t *dest, int idx, unsigned int nb_bits, const uint8_t *tab_byte)
{
    unsigned int i;
    int shift = 0;

    for (i = idx; i < idx + nb_bits; i++) {
        dest[i] = tab_byte[(i - idx) / 8] & (1 << shift) ? 1 : 0;
        /* gcc doesn't like: shift = (++shift) % 8; */
        shift++;
        shift %= 8;
    }
}

/* Gets the byte value from many bits.
   To obtain a full byte, set nb_bits to 8. */
uint8_t modbus_data::modbus_get_byte_from_bits(const uint8_t *src, int idx, unsigned int nb_bits)
{
    unsigned int i;
    uint8_t value = 0;

    if (nb_bits > 8) {
        /* Assert is ignored if NDEBUG is set */
        assert(nb_bits < 8);
        nb_bits = 8;
    }

    for (i=0; i < nb_bits; i++) {
        value |= (src[idx+i] << i);
    }

    return value;
}

/* Get a float from 4 bytes (Modbus) without any conversion (ABCD) */
float modbus_data::modbus_get_float_abcd(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = ntohl(((uint32_t)src[0] << 16) + src[1]);
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* Get a float from 4 bytes (Modbus) in inversed format (DCBA) */
float modbus_data::modbus_get_float_dcba(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = ntohl(bswap_32((((uint32_t)src[0]) << 16) + src[1]));
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* Get a float from 4 bytes (Modbus) with swapped bytes (BADC) */
float modbus_data::modbus_get_float_badc(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = ntohl((uint32_t)(bswap_16(src[0]) << 16) + bswap_16(src[1]));
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* Get a float from 4 bytes (Modbus) with swapped words (CDAB) */
float modbus_data::modbus_get_float_cdab(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = ntohl((((uint32_t)src[1]) << 16) + src[0]);
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* DEPRECATED - Get a float from 4 bytes in sort of Modbus format */
float modbus_data::modbus_get_float(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* Set a float to 4 bytes for Modbus w/o any conversion (ABCD) */
void modbus_data::modbus_set_float_abcd(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    i = htonl(i);
    dest[0] = (uint16_t)(i >> 16);
    dest[1] = (uint16_t)i;
}

/* Set a float to 4 bytes for Modbus with byte and word swap conversion (DCBA) */
void modbus_data::modbus_set_float_dcba(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    i = bswap_32(htonl(i));
    dest[0] = (uint16_t)(i >> 16);
    dest[1] = (uint16_t)i;
}

/* Set a float to 4 bytes for Modbus with byte swap conversion (BADC) */
void modbus_data::modbus_set_float_badc(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    i = htonl(i);
    dest[0] = (uint16_t)bswap_16(i >> 16);
    dest[1] = (uint16_t)bswap_16(i & 0xFFFF);
}

/* Set a float to 4 bytes for Modbus with word swap conversion (CDAB) */
void modbus_data::modbus_set_float_cdab(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    i = htonl(i);
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

/* DEPRECATED - Set a float to 4 bytes in a sort of Modbus format! */
void modbus_data::modbus_set_float(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

