#include "Common.h"
#include "S_Interpreter.h"
#include "S_Context.h"
#include "S_Value.h"
#include "MM.h"
#include "DBG.h"

struct S_Global_Name {
    struct S_Global_Name* next;
    char* name;
};

struct S_Context {
    struct S_Context* next;
    struct S_Local_Variables* variables;
    struct S_Global_Name* global_name;
};

void S_ContextPush(struct S_Interpreter* interpreter)
{
    struct S_Context* context = 
        (struct S_Context*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Context));

    DCHECK(context != 0);
    memset(context, 0, sizeof(struct S_Context));

    context->next = interpreter->Context;
    context->variables = 0;

    interpreter->Context = context;

    // mark the context gc-collectable.
    MM_MarkGCMemoryCollectable(interpreter->RunningStorage, context);
}

void S_ContextPop(struct S_Interpreter* interpreter)
{
    DCHECK(interpreter != 0);
    DCHECK(interpreter->Context != 0);

    interpreter->Context = interpreter->Context->next;

    // Need not release context, it would be collected by gc an the end.
}

struct S_Local_Variables* GetVariable(struct S_Interpreter* interpreter, const char* name, int only_local)
{
    DCHECK(interpreter != 0);
    DCHECK(interpreter->Context != 0);

    struct S_Context* context = interpreter->Context;
    struct S_Local_Variables* var_founded = 0;
    while (context != 0)
    {
        S_Local_Variables* current_variable = context->variables;
        while (current_variable != 0)
        {
            if (strcmp(current_variable->name, name) == 0)
            {
                var_founded = current_variable;
                break;
            }

            current_variable = current_variable->next;
        }

        // ok we found.
        if (var_founded != 0)
        {
            break;
        }

        // not found and only search local context.
        if (only_local != 0)
        {
            break;
        }

        // search next context
        context = context->next;
    }

    return var_founded;
}

struct S_Local_Variables* S_ContextFindVariable(struct S_Interpreter* interpreter, const char* name, int only_local, int create_if_not_found)
{
    struct S_Local_Variables* var = GetVariable(interpreter, name, only_local);
    if (var != 0)
    {
        return var;
    }

    if (create_if_not_found == 0)
    {
        return 0;
    }

    // need create an new variable in local context.
    var = (struct S_Local_Variables*)MM_Malloc(interpreter->RunningStorage, 
                                               sizeof(struct S_Local_Variables));
    var->next = interpreter->Context->variables;
    var->name = MM_CopyString(interpreter->RunningStorage, name);
    var->value = 0;

    interpreter->Context->variables = var;

    MM_MarkGCMemoryCollectable(interpreter->RunningStorage, var->name);
    MM_MarkGCMemoryCollectable(interpreter->RunningStorage, var);
	return var;
}

void S_ContextMarkVarGlobal(struct S_Interpreter* interpreter, char* name)
{
    DCHECK(interpreter != 0);
    DCHECK(interpreter->Context != 0);

    struct S_Context* context = interpreter->Context;
    struct S_Global_Name* global_name = (struct S_Global_Name*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Global_Name));
    global_name->next = context->global_name;
    global_name->name = MM_CopyString(interpreter->RunningStorage, name);

    context->global_name = global_name;

    MM_MarkGCMemoryCollectable(interpreter->RunningStorage, global_name->name);
    MM_MarkGCMemoryCollectable(interpreter->RunningStorage, global_name);
}

int S_ContextIsGlobalVar(struct S_Interpreter* interpreter, const char* name)
{
    DCHECK(interpreter != 0);
    DCHECK(interpreter->Context != 0);

    struct S_Context* context = interpreter->Context;
    struct S_Global_Name* global_name = context->global_name;

    while (global_name != 0)
    {
        if (strcmp(global_name->name, name) == 0)
        {
            return 1;
        }

        global_name = global_name->next;
    }

    return 0;
}

