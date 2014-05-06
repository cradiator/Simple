#pragma once

#include "Common.h"
#include "MM.h"

struct S_Statement_List;
struct S_Context;

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

    struct S_Statement_List* StatementList;

    // local variable live in context.
    // GC-Collectable.
    struct S_Context* Context;

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
EXTERN_C void S_IncSrcLineNo(struct S_Interpreter* interpreter);

// Get current source line no. used by Simple.l
EXTERN_C int S_GetSrcLineNo(struct S_Interpreter* interpreter);

EXTERN_C void yyerror(struct S_Interpreter* interpreter, const char* message, ...);

