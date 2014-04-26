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


};

// Create an interpreter engine.
struct S_Interpreter* S_NewInterpreter();

