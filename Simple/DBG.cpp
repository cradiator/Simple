#include "Common.h"

void DBG_Panic(const char* filename, int lineno, const char* msg)
{
    fprintf(stderr, "%s:%d %s\n", filename, lineno, msg);
    exit(-1);
}

void DBG_Assert(bool exp, const char* filename, int lineno, const char* msg)
{
    if (exp)
    {
        DBG_Panic(filename, lineno, msg);
    }
}

void DBG_FormatPanic(const char* filename, int lineno, const char* format, ...)
{
    char buffer[0x100] = { 0 };

    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    DBG_Panic(filename, lineno, buffer);
}
