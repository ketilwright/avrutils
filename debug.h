#pragma once
#include <stdint.h>

enum DbgFilter 
{
    dbg_error       = 0x00000001,
    dbg_warn        = 0x00000002,
    dbg_print       = 0x00000004,
    dbg_info        = 0x00000008,
    dbg_reserved    = 0x80000000,
    dbg_all         = 0xffffffff
};

#if defined (NDEBUG)
#define DbgPrint(filter, fmt, ...)
#else
extern uint32_t g_dbgFilter;
void _DbgPrint(const char *fmt,...);
#define DbgPrint(filter, fmt, ...) if(g_dbgFilter & (filter)) { _DbgPrint(fmt, ##__VA_ARGS__) ;}

#endif

