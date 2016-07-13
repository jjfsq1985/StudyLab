#ifndef MODBUS_VERSION_H
#define MODBUS_VERSION_H


#define ASIOMODBUS_VERSION_MAJOR (0)

#define ASIOMODBUS_VERSION_MINOR (1)

#define ASIOMODBUS_VERSION_MICRO (1)

#define ASIOMODBUS_VERSION 0.0.1

/* Numerically encoded version, like 0x010203 */
#define ASIOMODBUS_VERSION_HEX    ((ASIOMODBUS_VERSION_MAJOR << 24) | \
                                                                          (ASIOMODBUS_VERSION_MAJOR << 16) |  \
                                                                          (ASIOMODBUS_VERSION_MICRO << 8))

#define ASIOMODBUS_VERSION_STRING "0.0.1"

/* Evaluates to True if the version is greater than @major, @minor and @micro
*/
#define ASIOMODBUS_VERSION_CHECK(major,minor,micro)      \
    (ASIOMODBUS_VERSION_MAJOR > (major) ||               \
     (ASIOMODBUS_VERSION_MAJOR == (major) &&             \
      ASIOMODBUS_VERSION_MINOR > (minor)) ||             \
     (ASIOMODBUS_VERSION_MAJOR == (major) &&             \
      ASIOMODBUS_VERSION_MINOR == (minor) &&             \
      ASIOMODBUS_VERSION_MICRO >= (micro)))

#endif /* MODBUS_VERSION_H */

