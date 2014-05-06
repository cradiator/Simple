#include "Common.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "S_Expression.h"

#include "MM.h"
#include "DBG.h"

struct S_Value* S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp)
{
	return NULL;
}

struct S_Value* S_Eval_Expression_Nil(struct S_Interpreter* interpreter, struct S_Expression_Nil* exp)
{
    return (struct S_Value*)S_CreateValueNil(interpreter);
}

struct S_Value* S_Eval_Expression_Integer(struct S_Interpreter* interpreter, struct S_Expression_Integer* exp)
{
    return (struct S_Value*)S_CreateValueInteger(interpreter, exp->value);
}

struct S_Value* S_Eval_Expression_Double(struct S_Interpreter* interpreter, struct S_Expression_Double* exp)
{
    return (struct S_Value*)S_CreateValueDouble(interpreter, exp->value);
}

struct S_Value* S_Eval_Expression_True(struct S_Interpreter* interpreter, struct S_Expression_True* exp)
{
    return (struct S_Value*)S_CreateValueTrue(interpreter);
}

struct S_Value* S_Eval_Expression_False(struct S_Interpreter* interpreter, struct S_Expression_False* exp)
{
    return (struct S_Value*)S_CreateValueFalse(interpreter);
}

struct S_Value* S_Eval_Expression_String(struct S_Interpreter* interpreter, struct S_Expression_String* exp)
{
    char* buffer = (char*)MM_Malloc(interpreter, exp->length + 1);
    memcpy(buffer, exp->string, exp->length);
    buffer[exp->length] = 0;

    return (struct S_Value*)S_CreateValueString(interpreter, buffer);
}


