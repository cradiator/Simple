#include "Common.h"
#include "MM.h"
#include "DBG.h"
#include "S_Expression.h"
#include "S_Interpreter.h"

//// Expression /////
struct S_Expression_Nil* S_CreateExpressionNil(S_Interpreter* interpreter)
{
    struct S_Expression_Nil* e = 
        (struct S_Expression_Nil*)MM_AllocateStorage(interpreter->ParsingStorage, 
                                                      sizeof(struct S_Expression_Nil));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_NIL;
    return e;
}

struct S_Expression_Integer* S_CreateExpressionInteger(S_Interpreter* interpreter, int value)
{
    struct S_Expression_Integer* e = 
        (struct S_Expression_Integer*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                         sizeof(struct S_Expression_Integer));
    DCHECK(e != 0);
    e->header.type = EXPRESSION_TYPE_INTEGER;
    e->value = value;
    return e;
}

struct S_Expression_Double* S_CreateExpressionDouble(S_Interpreter* interpreter, double value)
{
    struct S_Expression_Double* e =
        (struct S_Expression_Double*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                        sizeof(struct S_Expression_Double));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_DOUBLE;
    e->value = value;
    return e;
}

struct S_Expression_String* S_CreateExpressionString(struct S_Interpreter* interpreter, char* string)
{
    DCHECK(string != 0);

    struct S_Expression_String* e =
        (struct S_Expression_String*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                        sizeof(struct S_Expression_String));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_STRING;
    e->length = strlen(string);
    e->string = string;

    return e;
}

struct S_Expression_Symbol* S_CreateExpressionSymbol(struct S_Interpreter* interpreter, char* symbol)
{
    DCHECK(symbol != 0);

    struct S_Expression_Symbol* e =
        (struct S_Expression_Symbol*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                        sizeof(struct S_Expression_Symbol));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_SYMBOL;
    e->symbol = symbol;
    return e;
}

struct S_Expression_Negation* S_CreateExpressionNegation(struct S_Interpreter* interpreter, struct S_Expression* exp)
{
    DCHECK(exp != 0);

    struct S_Expression_Negation* e =
        (struct S_Expression_Negation*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                          sizeof(struct S_Expression_Negation));

    DCHECK(e != 0);
    e->header.type = EXPRESSION_TYPE_NEGATION;
    e->exp = exp;

    return e;
}

struct S_Expression_Function_Call* S_CreateExpressionFunctionCall(struct S_Interpreter* interpreter, 
                                                                   struct S_Expression *fn, 
                                                                   struct S_Expression_List* param)
{
    DCHECK(interpreter != 0);
    DCHECK(fn != 0);
    DCHECK(param != 0);

    struct S_Expression_Function_Call* e =
        (struct S_Expression_Function_Call*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                               sizeof(struct S_Expression_Function_Call));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_FUNCTION_CALL;
    e->fn = fn;
    e->param = param;

    return e;
}

struct S_Expression_Op2* S_CreateExpressionOp2(struct S_Interpreter* interpreter,
                                               int op,
                                               struct S_Expression* exp1,
                                               struct S_Expression* exp2)
{
    DCHECK(interpreter != 0);
    DCHECK(op >= OP2_MIN && op < OP2_MAX);
    DCHECK(exp1 != 0);
    DCHECK(exp2 != 0);

    struct S_Expression_Op2 *e =
        (struct S_Expression_Op2*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Expression_Op2));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_OP2;
    e->op = op;
    e->exp1 = exp1;
    e->exp2 = exp2;
    return e;
}

//// End of Expression ////

//// Expression List ////
struct S_Expression_List* S_CreateExpList(struct S_Interpreter* interpreter, struct S_Expression* exp)
{
    DCHECK(interpreter != 0);

    struct S_Expression_List* exp_list =
        (struct S_Expression_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                      sizeof(struct S_Expression_List));
    DCHECK(exp_list != 0);

    exp_list->next = NULL;
    exp_list->exp = exp;
    return exp_list;
}

void S_AddExpressionToExpList(struct S_Interpreter* interpreter, struct S_Expression_List* exp_list, struct S_Expression* exp)
{
    DCHECK(interpreter != 0);
    DCHECK(exp_list != 0);
    DCHECK(exp != 0);

    if (exp_list->exp == 0)
    {
        DCHECK(exp_list->next == 0);
        exp_list->exp = exp;
        return;
    }

    // create a new node.
    struct S_Expression_List* next =
        (struct S_Expression_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                      sizeof(struct S_Expression_List));
    next->next = NULL;
    next->exp = exp;

    // push back the new node.
    while (exp_list->next != 0)
        exp_list = exp_list->next;

    exp_list->next = next;
}

//// End of Expression List ////

//// Parameter List ////
struct S_Parameter_List* S_CreateParamList(struct S_Interpreter* interpreter, struct S_Expression_Symbol* param)
{
    DCHECK(interpreter != 0);

    struct S_Parameter_List* param_list =
        (struct S_Parameter_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Parameter_List));
    DCHECK(param_list != 0);

    param_list->next = NULL;
    param_list->symbol = param;
    return param_list;
}

void S_AddParameterToParamList(struct S_Interpreter* interpreter, struct S_Parameter_List* param_list, struct S_Expression_Symbol* param)
{
    DCHECK(interpreter != 0);
    DCHECK(param_list != 0);
    DCHECK(param != 0);

    if (param_list->symbol == 0)
    {
        DCHECK(param_list->next == 0);
        param_list->symbol = param;
        return;
    }

    // create a new node.
    struct S_Parameter_List* next =
        (struct S_Parameter_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Parameter_List));
    next->next = NULL;
    next->symbol = param;

    // push back the new node.
    while (param_list->next != 0)
        param_list = param_list->next;

    param_list->next = next;
}

//// End of Parameter List ////

