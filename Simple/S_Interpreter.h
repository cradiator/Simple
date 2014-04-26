#pragma once

#include "MM.h"

struct S_Interpreter
{
	struct MM_Storage*   ParsingStorage;
	struct MM_GCStorage* RunningStorage;
};

// Create an interpreter engine.
struct S_Interpreter* S_NewInterpreter();

