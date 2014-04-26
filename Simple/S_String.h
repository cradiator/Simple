#pragma once

#include "S_Interpreter.h"

void S_OpenString(struct S_Interpreter* interpreter);

void S_AddStringChar(struct S_Interpreter* interpreter, char c);

char* S_CloseString(struct S_Interpreter* interpreter);
