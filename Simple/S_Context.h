#pragma once


void S_ContextPush(struct S_Interpreter* interpreter);

void S_ContextPop(struct S_Interpreter* interpreter);

// Mark an variable name global in top context.
void S_ContextMarkVarGlobal(struct S_Interpreter* interpreter, const char* name);

// Is name global in top context?
// return 1 if it is, 0 if not.
int S_ContextIsGlobalVar(struct S_Interpreter* interpreter, const char* name);

/// Local Variables
struct S_Local_Variables {
    struct S_Local_Variables* next;
    char*  name;
    struct S_Value* value;
};

// Find the local variables in context.
// If only_local IS NOT 0, only search in top context, 
// if is 0 S_ContextFindVariable would walk through all context.
//
// return 0, if variable is not found.
struct S_Local_Variables* S_ContextFindVariable(struct S_Interpreter* interpreter, const char* name, int only_local, int create_if_not_found);

