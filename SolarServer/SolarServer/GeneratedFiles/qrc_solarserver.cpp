/****************************************************************************
** Resource object code
**
** Created by: The Resource Compiler for Qt version 5.6.0
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

static const unsigned char qt_resource_data[] = {
  // D:/Work_PRJ/SolarServer/SolarServer/SolarServer.rc
  0x0,0x0,0x0,0x32,
  0x49,
  0x44,0x49,0x5f,0x49,0x43,0x4f,0x4e,0x31,0x9,0x9,0x49,0x43,0x4f,0x4e,0x9,0x9,
  0x44,0x49,0x53,0x43,0x41,0x52,0x44,0x41,0x42,0x4c,0x45,0x9,0x22,0x53,0x6f,0x6c,
  0x61,0x72,0x53,0x65,0x72,0x76,0x65,0x72,0x2e,0x69,0x63,0x6f,0x22,0xd,0xa,0xd,
  0xa,
  
};

static const unsigned char qt_resource_name[] = {
  // SolarServer
  0x0,0xb,
  0x7,0x28,0x59,0xc2,
  0x0,0x53,
  0x0,0x6f,0x0,0x6c,0x0,0x61,0x0,0x72,0x0,0x53,0x0,0x65,0x0,0x72,0x0,0x76,0x0,0x65,0x0,0x72,
    // SolarServer.rc
  0x0,0xe,
  0x5,0x9c,0xb0,0x83,
  0x0,0x53,
  0x0,0x6f,0x0,0x6c,0x0,0x61,0x0,0x72,0x0,0x53,0x0,0x65,0x0,0x72,0x0,0x76,0x0,0x65,0x0,0x72,0x0,0x2e,0x0,0x72,0x0,0x63,
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
  // :/SolarServer
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,
  // :/SolarServer/SolarServer.rc
  0x0,0x0,0x0,0x1c,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,

};

#ifdef QT_NAMESPACE
#  define QT_RCC_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::name
#  define QT_RCC_MANGLE_NAMESPACE0(x) x
#  define QT_RCC_MANGLE_NAMESPACE1(a, b) a##_##b
#  define QT_RCC_MANGLE_NAMESPACE2(a, b) QT_RCC_MANGLE_NAMESPACE1(a,b)
#  define QT_RCC_MANGLE_NAMESPACE(name) QT_RCC_MANGLE_NAMESPACE2( \
        QT_RCC_MANGLE_NAMESPACE0(name), QT_RCC_MANGLE_NAMESPACE0(QT_NAMESPACE))
#else
#   define QT_RCC_PREPEND_NAMESPACE(name) name
#   define QT_RCC_MANGLE_NAMESPACE(name) name
#endif

#ifdef QT_NAMESPACE
namespace QT_NAMESPACE {
#endif

bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);

bool qUnregisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);

#ifdef QT_NAMESPACE
}
#endif

int QT_RCC_MANGLE_NAMESPACE(qInitResources_solarserver)();
int QT_RCC_MANGLE_NAMESPACE(qInitResources_solarserver)()
{
    QT_RCC_PREPEND_NAMESPACE(qRegisterResourceData)
        (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

int QT_RCC_MANGLE_NAMESPACE(qCleanupResources_solarserver)();
int QT_RCC_MANGLE_NAMESPACE(qCleanupResources_solarserver)()
{
    QT_RCC_PREPEND_NAMESPACE(qUnregisterResourceData)
       (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

namespace {
   struct initializer {
       initializer() { QT_RCC_MANGLE_NAMESPACE(qInitResources_solarserver)(); }
       ~initializer() { QT_RCC_MANGLE_NAMESPACE(qCleanupResources_solarserver)(); }
   } dummy;
}
