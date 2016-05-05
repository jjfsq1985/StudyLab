#include <winsock2.h>
#include <assert.h>
#include <QtWidgets>

#define OCI_CHARSET_WIDE

void __cdecl Tprintf(const wchar_t *format, ...);
void __cdecl Aprintf(const char *format, ...);