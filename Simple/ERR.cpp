#include "Common.h"
#include "ERR.h"

void ERR_Print(int level, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
	ERR_VPrint(level, format, ap);
    va_end(ap);
}

void ERR_VPrint(int level, const char* format, va_list ap)
{
	vfprintf(stderr, format, ap);
}
