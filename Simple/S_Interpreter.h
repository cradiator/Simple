#pragma once

#include "MM.h"

enum 
{
	INTERPRETER_FLAG_OPENSTRING  = 0x01,
    INTERPRETER_FLAG_HAVE_SOURCE = 0x02,
};

struct S_Interpreter
{
	struct MM_Storage*   ParsingStorage;
	struct MM_GCStorage* RunningStorage;

	unsigned int Flag;

    // used by S_DoCompileFile.
    char* SourceFileContent;
    int SourceSize;

	// used by S_OpenString/S_AddStringChar/S_CloseString
	void* PendingString;

    // used by S_IncSrcLineNo / S_GetSrcLineNo when compiling.
	int LineNo;
};

// Create an interpreter engine.
struct S_Interpreter* S_NewInterpreter();

// Comple an source file.
// return 0 if success, else error code.
int S_DoCompileFile(struct S_Interpreter* interpreter, const char* filename);

// Increment source line no. used by Simple.l
void S_IncSrcLineNo(struct S_Interpreter* interpreter);

// Get current source line no. used by Simple.l
int S_GetSrcLineNo(struct S_Interpreter* interpreter);

