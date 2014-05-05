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

struct S_Value_String* S_CreateValueString(struct S_Interpreter* interpreter, char* string)
{
    DCHECK(string != 0);

    struct S_Value_String* v =
        (struct S_Value_String*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_String));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_STRING;
    v->string = string;
    return v;
}

struct S_Value_Symbol* S_CreateValueSymbol(struct S_Interpreter* interpreter, char* symbol)
{
    DCHECK(symbol != 0);
    struct S_Value_Symbol* v =
        (struct S_Value_Symbol*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Symbol));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_SYMBOL;
    v->symbol = symbol;
    return v;
}

struct S_Value_Function* S_CreateValueFunction(struct S_Interpreter* interpreter, struct S_Expression_Function_Define* function)
{
    DCHECK(function != 0);
    struct S_Value_Function* v =
        (struct S_Value_Function*)MM_Malloc(interpreter->RunningStorage, sizeof(S_Value_Function));
    DCHECK(v != 0);

    v->header.type = VALUE_TYPE_FUNCTION;
    v->function = function;

    return v;
}

