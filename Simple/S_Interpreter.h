#pragma once

#include "MM.h"

enum 
{
	INTERPRETER_FLAG_OPENSTRING = 0x01,
};

struct S_Interpreter
{
	struct MM_Storage*   ParsingStorage;
	struct MM_GCStorage* RunningStorage;

	unsigned int Flag;

	// used by S_OpenString/S_AddStringChar/S_CloseString
	void* PendingString;

	int LineNo;
};

// Create an interpreter engine.
struct S_Interpreter* S_NewInterpreter();

// Increment source line no. used by Simple.l
void S_IncSrcLineNo(struct S_Interpreter* interpreter);

// Get current source line no. used by Simple.l
int S_GetSrcLineNo(struct S_Interpreter* interpreter);

