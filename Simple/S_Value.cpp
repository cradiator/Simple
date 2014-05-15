#include "Common.h"
#include "MM.h"
#include "DBG.h"
#include "S_Interpreter.h"
#include "S_Value.h"

struct S_Value_Nil* S_CreateValueNil(struct S_Interpreter* interpreter)
{
    struct S_Value_Nil* v = 
        (struct S_Value_Nil*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Nil));

    DCHECK(v != 0);
    v->header.type = VALUE_TYPE_NIL;

    return v;
}


struct S_Value_True* S_CreateValueTrue(struct S_Interpreter* interpreter)
{
    struct S_Value_True* v = 
        (struct S_Value_True*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_True));

    DCHECK(v != 0);
    v->header.type = VALUE_TYPE_TRUE;

    return v;
}

struct S_Value_False* S_CreateValueFalse(struct S_Interpreter* interpreter)
{
    struct S_Value_False* v = 
        (struct S_Value_False*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_False));

    DCHECK(v != 0);
    v->header.type = VALUE_TYPE_FALSE;

    return v;
}

struct S_Value_Integer* S_CreateValueInteger(struct S_Interpreter* interpreter, int value)
{
    struct S_Value_Integer* v = 
        (struct S_Value_Integer*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Integer));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_INTEGER;
    v->value = value;
    return v;
}

struct S_Value_Double* S_CreateValueDouble(struct S_Interpreter* interpreter, double value)
{
    struct S_Value_Double* v = 
        (struct S_Value_Double*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Double));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_DOUBLE;
    v->value = value;
    return v;
}

struct S_Value_String* S_CreateValueString(struct S_Interpreter* interpreter, const char* string)
{
    DCHECK(string != 0);

    struct S_Value_String* v =
        (struct S_Value_String*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_String));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_STRING;
    v->string = MM_CopyString(interpreter->RunningStorage, string);
    v->length = strlen(v->string);
    return v;
}

struct S_Value_Symbol* S_CreateValueSymbol(struct S_Interpreter* interpreter, const char* symbol)
{
    DCHECK(symbol != 0);
    struct S_Value_Symbol* v =
        (struct S_Value_Symbol*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Symbol));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_SYMBOL;
    v->symbol = MM_CopyString(interpreter->RunningStorage, symbol);
    return v;
}

struct S_Value_Function* S_CreateValueFunction(struct S_Interpreter* interpreter, struct S_Parameter_List* param_list, struct S_Code_Block* code_block)
{
    struct S_Value_Function* v =
        (struct S_Value_Function*)MM_Malloc(interpreter->RunningStorage, sizeof(S_Value_Function));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_FUNCTION;
    v->type = SCRIPT_FUNCTION;
    v->u.script.param_list = param_list;
    v->u.script.code_block = code_block;

    return v;
}

struct S_Value_Function* S_CreateValueNativeFunction(struct S_Interpreter* interpreter, S_NativeFunctionProto function)
{
    DCHECK(function != 0);
    struct S_Value_Function* v =
        (struct S_Value_Function*)MM_Malloc(interpreter->RunningStorage, sizeof(S_Value_Function));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_FUNCTION;
    v->type = NATIVE_FUNCTION;
    v->u.native = function;

    return v;
}

struct S_Value_Array* S_CreateValueArray(struct S_Interpreter* interpreter, struct S_Value** value_array, unsigned int array_size)
{
    DCHECK(value_array != 0 || array_size == 0);

    struct S_Value_Array* v = 
        (struct S_Value_Array*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Array));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_ARRAY;
    v->value_array = value_array;
    v->array_size = array_size;
    return v;
}

void S_MarkValueCollectable(struct S_Interpreter* interpreter, struct S_Value* value)
{
    DCHECK(value != 0);

    if (value->header.type == VALUE_TYPE_STRING)
    {
        struct S_Value_String* s = (struct S_Value_String*)value;
        MM_MarkGCMemoryCollectable(interpreter->RunningStorage, s->string);
    }
    else if (value->header.type == VALUE_TYPE_SYMBOL)
    {
        struct S_Value_Symbol* s = (struct S_Value_Symbol*)value;
        MM_MarkGCMemoryCollectable(interpreter->RunningStorage, s->symbol);
    }
    else if (value->header.type == VALUE_TYPE_ARRAY)
    {
        struct S_Value_Array* s = (struct S_Value_Array*)value;
        if (s->value_array != 0)
            MM_MarkGCMemoryCollectable(interpreter->RunningStorage, s->value_array);
        for(unsigned int i = 0; i < s->array_size; ++i)
            S_MarkValueCollectable(interpreter, (s->value_array)[i]);
    }

    MM_MarkGCMemoryCollectable(interpreter->RunningStorage, value);
}

void S_MarkValue(struct S_Interpreter* interpreter, struct S_Value* value)
{
    DCHECK(value != 0);

    if (value->header.type == VALUE_TYPE_STRING)
    {
        struct S_Value_String* s = (struct S_Value_String*)value;
        MM_MarkGCMemory(interpreter->RunningStorage, s->string);
    }
    else if (value->header.type == VALUE_TYPE_SYMBOL)
    {
        struct S_Value_Symbol* s = (struct S_Value_Symbol*)value;
        MM_MarkGCMemory(interpreter->RunningStorage, s->symbol);
    }
    else if (value->header.type == VALUE_TYPE_ARRAY)
    {
        struct S_Value_Array* s = (struct S_Value_Array*)value;
        if (s->value_array != 0)
            MM_MarkGCMemory(interpreter->RunningStorage, s->value_array);
        for(unsigned int i = 0; i < s->array_size; ++i)
            S_MarkValue(interpreter, (s->value_array)[i]);
    }

    MM_MarkGCMemory(interpreter->RunningStorage, value);
}
