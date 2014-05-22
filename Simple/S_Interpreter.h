#pragma once

#include "Common.h"
#include "MM.h"
#include "S_Expression.h"
#include "S_Value.h"

struct S_Statement_List;
struct S_Context;
struct S_Value;

enum 
{
	INTERPRETER_FLAG_OPENSTRING    = 0x01,
    INTERPRETER_FLAG_HAVE_SOURCE   = 0x02,
    INTERPRETER_FLAG_HAVE_RETURN = 0x04,
};

struct S_Interpreter
{
	struct MM_Storage*   ParsingStorage;
	struct MM_GCStorage* RunningStorage;

	unsigned int Flag;

    struct S_Statement_List* StatementList;

    // RETURN statement's result would save in this field.
    // Only valid when INTERPRETER_FLAG_HAVE_RETURNED is set.
    struct S_Value* ReturnValue;

    // local variable live in context.
    // GC-Collectable.
    struct S_Context* Context;

    struct S_Value_Function* NativePrint;
    struct S_Value_Function* NativeToString;

    void* RuntimeStack;

    // used by S_DoCompileFile.
    char* SourceFileContent;
    int SourceSize;

	// used by S_OpenString/S_AddStringChar/S_CloseString
	void* PendingString;

    // used by S_IncSrcLineNo / S_GetSrcLineNo when compiling.
	int LineNo;

    // used by S_GCIfNeed.
    unsigned int GCWaterMark;
};

// Create an interpreter engine.
struct S_Interpreter* S_NewInterpreter();

// Comple an source file.
// return 0 if success, else error code.
int S_DoCompileFile(struct S_Interpreter* interpreter, const char* filename);

// Run the compiled script.
// Call this method after S_DoCompileFile.
int S_Run(struct S_Interpreter* interpreter);

// Gubbage collect.
void S_GCIfNeed(struct S_Interpreter* interpreter);

#ifdef __cplusplus
// Create an native function.
bool S_AddNativeFunction(struct S_Interpreter* interpreter, const char* name, S_NativeFunctionProto native_function);
#endif

// Used by S_Eval_Statement_Return.
void S_SetReturnValue(struct S_Interpreter* interpreter, struct S_Value* value);

// Used by S_Eval_Expression_Function.
// Only valid when INTERPRETER_FLAG_HAVE_RETURN is set.
// return 0 if error.
struct S_Value* S_GetReturnValue(struct S_Interpreter* interpreter);

// Used by S_Eval_Expression_Function.
void S_ClearReturnValue(struct S_Interpreter* interpreter);

#ifdef __cplusplus
// Does we have return value?
bool S_IsHaveReturnValue(struct S_Interpreter* interpreter);
#endif

// Push a value into runtime stack
void S_PushRuntimeStackValue(struct S_Interpreter* interpreter, struct S_Value* value);

// Pop the top value from runtime stack
struct S_Value* S_PopRuntimeStackValue(struct S_Interpreter* interpreter);

// Get stack size.
unsigned int S_GetRuntimeStackSize(struct S_Interpreter* interpreter);

// Get a value from stack but not pop it.
// index == 0 get the top value, 1 get the second top value, and so on.
// The valid index range is [0, stack_size)
// return 0 if failed, value pointer if success.
struct S_Value* S_PeekRuntimeStackValue(struct S_Interpreter* interpreter, unsigned int index);

// Increment source line no. used by Simple.l
EXTERN_C void S_IncSrcLineNo(struct S_Interpreter* interpreter);

// Get current source line no. used by Simple.l
EXTERN_C int S_GetSrcLineNo(struct S_Interpreter* interpreter);

EXTERN_C void yyerror(struct S_Interpreter* interpreter, const char* message, ...);


