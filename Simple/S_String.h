#pragma once

#include "Common.h"
#include "S_Interpreter.h"

// when lex meet an string start quote
// call this routine to start buffer the string.
EXTERN_C void S_OpenString(struct S_Interpreter* interpreter);

// Add an char into string buffer.
EXTERN_C void S_AddStringChar(struct S_Interpreter* interpreter, char c);

// When lex meet the string end quote.
// Call this routine to generate the string.
EXTERN_C char* S_CloseString(struct S_Interpreter* interpreter);

// Allocate an string in parsing storage.
EXTERN_C char* S_AllocateStringInParsingStorage(struct S_Interpreter* interpreter, const char* string);

