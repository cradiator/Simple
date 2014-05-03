#pragma once

enum {
    ERR_CODE_SUCCESS = 0,
    ERR_CODE_FILE_NOT_FOUND,
    ERR_CODE_OPEN_FILE_FAILED,
    ERR_CODE_INVALID_FILE_SIZE,
};

__declspec(selectany)
const char* ERR_MESSAGE[] = {
    "Success",
    "File not found",
    "Open file failed",
    "Invalid file size",
};

enum {
    ERR_LEVEL_DEBUG,
    ERR_LEVEL_INFO,
    ERR_LEVEL_WRANING,
    ERR_LEVEL_ERROR,
};

void ERR_Print(int level, const char* format, ...);


