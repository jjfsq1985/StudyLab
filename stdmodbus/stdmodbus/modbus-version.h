/*
 * Copyright ? 2010-2014 Stephane Raimbault <stephane.raimbault@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *jiangjf <jjfsq1985@gmail.com> refactor to c++ from libmodbus-3.1.4
 */

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
