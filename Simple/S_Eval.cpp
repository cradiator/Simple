#include "Common.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "S_Expression.h"
#include "S_Context.h"

#include "MM.h"
#include "DBG.h"
#include "ERR.h"

struct S_Value* S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp);

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
	return (struct S_Value*)S_CreateValueString(interpreter, exp->string);
}

struct S_Value* S_Eval_Expression_Symbol(struct S_Interpreter* interpreter, struct S_Expression_Symbol* exp)
{
    int only_local = S_ContextIsGlobalVar(interpreter, exp->symbol) == 0 ? 1 : 0;
    S_Local_Variables* var = S_ContextFindVariable(interpreter, exp->symbol, only_local, 0);
	if (var != 0 && var->value != 0)
    {
        return var->value;
    }

    // value not found. mark error.
	ERR_Print(ERR_LEVEL_ERROR, "Line %d: symbol %s is not initialized.", exp->header.lineno, exp->symbol);
    return 0;
}

struct S_Value* EvalAdd(struct S_Interpreter* interpreter, int lineno, struct S_Value* left, struct S_Value* right)
{
    // For string concatenate.
    if (left->header.type == VALUE_TYPE_STRING || right->header.type == VALUE_TYPE_STRING)
    {
        if (left->header.type != right->header.type)
        {
            goto __ERROR_EXIT;
        }

        struct S_Value_String* left_string  = (struct S_Value_String*)left;
        struct S_Value_String* right_string = (struct S_Value_String*)right;
        int left_length  = strlen(left_string->string);
        int right_length = strlen(right_string->string);

        char* buffer = (char*)malloc(left_length + right_length + 1);
        memcpy(buffer, left_string->string, left_length);
        memcpy(buffer + left_length, right_string->string, right_length);
        buffer[left_length + right_length] = 0;

        struct S_Value* returned_value = (struct S_Value*)S_CreateValueString(interpreter, buffer);
        free(buffer);

        return returned_value;        
    }

    if (left->header.type != VALUE_TYPE_INTEGER && left->header.type != VALUE_TYPE_DOUBLE)
    {
        goto __ERROR_EXIT;
    }

    if (right->header.type != VALUE_TYPE_INTEGER && right->header.type != VALUE_TYPE_DOUBLE)
    {
        goto __ERROR_EXIT;
    }

    struct S_Value* returned_value = 0;
    if (left->header.type == VALUE_TYPE_INTEGER && right->header.type == VALUE_TYPE_INTEGER)
    {
        struct S_Value_Integer* left_integer  = (struct S_Value_Integer*)left;
        struct S_Value_Integer* right_integer = (struct S_Value_Integer*)right;
        returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_integer->value + right_integer->value);
    }
    else
    {
        double left_double  = left->header.type == VALUE_TYPE_INTEGER ? 
                              (double)(((struct S_Value_Integer*)left)->value) :
                              ((struct S_Value_Double*)left)->value;
        double right_double  = right->header.type == VALUE_TYPE_INTEGER ? 
                              (double)(((struct S_Value_Integer*)right)->value) :
                              ((struct S_Value_Double*)right)->value;

        returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, left_double + right_double);
    }

    return returned_value;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Lind %d: %s can not add %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return 0;
}

struct S_Value* EvalSubMulDiv(struct S_Interpreter* interpreter, int lineno, struct S_Value* left, struct S_Value* right, int op)
{
    DCHECK(op == OP2_SUB || op == OP2_MUL || op == OP2_DIV);

    if (left->header.type != VALUE_TYPE_INTEGER && left->header.type != VALUE_TYPE_DOUBLE)
    {
        goto __ERROR_EXIT;
    }

    if (right->header.type != VALUE_TYPE_INTEGER && right->header.type != VALUE_TYPE_DOUBLE)
    {
        goto __ERROR_EXIT;
    }

    struct S_Value* returned_value = 0;
    if (left->header.type == VALUE_TYPE_INTEGER && right->header.type == VALUE_TYPE_INTEGER)
    {
        struct S_Value_Integer* left_integer  = (struct S_Value_Integer*)left;
        struct S_Value_Integer* right_integer = (struct S_Value_Integer*)right;

        if (right_integer->value == 0 && op == OP2_DIV)
        {
            goto __ERROR_DIV_BY_ZERO;
        }

        if (op == OP2_SUB)
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_integer->value - right_integer->value);
        else if (op == OP2_MUL)
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_integer->value * right_integer->value);
        else if (op == OP2_DIV)
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_integer->value / right_integer->value);
    }
    else
    {
        double left_double  = left->header.type == VALUE_TYPE_INTEGER ? 
                              (double)(((struct S_Value_Integer*)left)->value) :
                              ((struct S_Value_Double*)left)->value;
        double right_double  = right->header.type == VALUE_TYPE_INTEGER ? 
                              (double)(((struct S_Value_Integer*)right)->value) :
                              ((struct S_Value_Double*)right)->value;

        if (right_double == 0 && op == OP2_DIV)
        {
            goto __ERROR_DIV_BY_ZERO;
        }

        if (op == OP2_SUB)
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_double - right_double);
        else if (op == OP2_MUL)
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_double * right_double);
        else if (op == OP2_DIV)
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, left_double / right_double);
    }

    return returned_value;

__ERROR_DIV_BY_ZERO:
    ERR_Print(ERR_LEVEL_ERROR,
              "Line %d: divided by zero.",
              lineno);
    return 0;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Lind %d: %s can not sub %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return 0;
}

struct S_Value* S_Eval_Expression_Op2(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
{
    S_Value* left  = 0;
    S_Value* right = 0;
    S_Value* returned_value = 0;


    left = S_Eval_Expression(interpreter, exp->exp1);
    if (left == 0)
        goto __EXIT;

    right = S_Eval_Expression(interpreter, exp->exp2);
    if (right == 0)
        goto __EXIT;
    
    switch(exp->op)
    {
        case OP2_ADD:
            returned_value = EvalAdd(interpreter, exp->header.lineno, left, right);
            break;

        case OP2_SUB:
        case OP2_MUL:
        case OP2_DIV:
            returned_value = EvalSubMulDiv(interpreter, exp->header.lineno, left, right, exp->op);
            break;

        default:
            break;
    }

__EXIT:
    if (left != 0)
        S_MarkValueCollectable(interpreter, left);
    if (right != 0)
        S_MarkValueCollectable(interpreter, right);
    return returned_value;
}

struct S_Value* S_Eval_Function_Call(struct S_Interpreter* interpreter, struct S_Expression_Function_Call* exp)
{
    /*
    struct S_Value* function_value = S_Eval_Expression(interpreter, exp->fn);
    if (function_value == 0 || function_value->)
    */

    // TODO: need implement statement evaluation first.
    return 0;
}


struct S_Value* S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp)
{
    DCHECK(interpreter != 0);
    DCHECK(exp != 0);

    struct S_Value* returned_value = 0;
    switch (exp->header.type)
    {
    case EXPRESSION_TYPE_NIL:
        returned_value = S_Eval_Expression_Nil(interpreter, (struct S_Expression_Nil*)exp);
        break;

    case EXPRESSION_TYPE_INTEGER:
        returned_value = S_Eval_Expression_Integer(interpreter, (struct S_Expression_Integer*)exp);
        break;

    case EXPRESSION_TYPE_DOUBLE:
        returned_value = S_Eval_Expression_Double(interpreter, (struct S_Expression_Double*)exp);
        break;

    case EXPRESSION_TYPE_TRUE:
        returned_value = S_Eval_Expression_True(interpreter, (struct S_Expression_True*)exp);
        break;

    case EXPRESSION_TYPE_FALSE:
        returned_value = S_Eval_Expression_False(interpreter, (struct S_Expression_False*)exp);
        break;

    case EXPRESSION_TYPE_STRING:
        returned_value = S_Eval_Expression_String(interpreter, (struct S_Expression_String*)exp);
        break;

    case EXPRESSION_TYPE_SYMBOL:
        returned_value = S_Eval_Expression_Symbol(interpreter, (struct S_Expression_Symbol*)exp);
        break;

    case EXPRESSION_TYPE_OP2:
        returned_value = S_Eval_Expression_Op2(interpreter, (struct S_Expression_Op2*)exp);
        break;
    }
    return NULL;
}