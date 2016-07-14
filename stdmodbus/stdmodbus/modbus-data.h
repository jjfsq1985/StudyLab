#ifndef MODBUS_DATA_H
#define MODBUS_DATA_H

class modbus_data
{
public:
    modbus_data();
    ~modbus_data();

public:
    static inline uint16_t bswap_16(uint16_t x)
    {
        return (x >> 8) | (x << 8);
    }

    static inline uint32_t bswap_32(uint32_t x)
    {
        return (bswap_16(x & 0xffff) << 16) | (bswap_16(x >> 16));
    }

public:
    static void modbus_set_bits_from_byte(uint8_t *dest, int idx, const uint8_t value);
    static void modbus_set_bits_from_bytes(uint8_t *dest, int idx, unsigned int nb_bits, const uint8_t *tab_byte);
    static uint8_t modbus_get_byte_from_bits(const uint8_t *src, int idx, unsigned int nb_bits);
    static float modbus_get_float_abcd(const uint16_t *src);
    static float modbus_get_float_dcba(const uint16_t *src);
    static float modbus_get_float_badc(const uint16_t *src);
    static float modbus_get_float_cdab(const uint16_t *src);
    static float modbus_get_float(const uint16_t *src);
    static void modbus_set_float_abcd(float f, uint16_t *dest);
    static void modbus_set_float_dcba(float f, uint16_t *dest);
    static void modbus_set_float_badc(float f, uint16_t *dest);
    static void modbus_set_float_cdab(float f, uint16_t *dest);
    static void modbus_set_float(float f, uint16_t *dest);
};

#endif
