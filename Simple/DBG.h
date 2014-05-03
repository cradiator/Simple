#pragma once

void DBG_Assert(bool exp, const char* filename, int lineno, const char* msg);
void DBG_Panic(const char* filename, int lineno, const char* msg);
void DBG_FormatPanic(const char* filename, int lineno, const char* format, ...);

#define DCHECK(exp) DBG_Assert((exp), __FILE__, __LINE__, "Assert Failed.")
#define DPANIC(msg) DBG_Panic(__FILE__, __LINE__, msg)
#define DPANIC_FORMAT(format, ...) DBG_FormatPanic(__FILE__, __LINE__, format, __VA_ARGS__)

