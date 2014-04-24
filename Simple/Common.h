#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define PAGE_SIZE (0x1000)
#define ALIGN_UP(Size, Align) ((((Size) + (Align) - 1) / (Align)) * (Align))

