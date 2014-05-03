#include "Common.h"
#include "ERR.h"

void ERR_Print(int level, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}

