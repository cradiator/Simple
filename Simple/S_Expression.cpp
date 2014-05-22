#include "Common.h"
#include "MM.h"
#include "DBG.h"
#include "S_Expression.h"
#include "S_Interpreter.h"

//// Expression /////
void InitializeExpressionHeader(S_Interpreter* interpreter, S_Expression* exp, enum S_Expression_Type type)
{
    exp->header.type = type;
    exp->header.lineno = S_GetSrcLineNo(interpreter);
}

struct S_Expression_Nil* S_CreateExpressionNil(S_Interpreter* interpreter)
{
    DCHECK(interpreter != 0);

    struct S_Expression_Nil* e = 
        (struct S_Expression_Nil*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Expression_Nil));
    DCHECK(e != 0);
    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_NIL);
    return e;
}

struct S_Expression_Integer* S_CreateExpressionInteger(S_Interpreter* interpreter, int value)
{
    struct S_Expression_Integer* e = 
        (struct S_Expression_Integer*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                         sizeof(struct S_Expression_Integer));
    DCHECK(e != 0);
    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_INTEGER);
    e->value = value;
    return e;
}

struct S_Expression_Double* S_CreateExpressionDouble(S_Interpreter* interpreter, double value)
{
    struct S_Expression_Double* e =
        (struct S_Expression_Double*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                        sizeof(struct S_Expression_Double));
    DCHECK(e != 0);

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_DOUBLE);
    e->value = value;
    return e;
}

struct S_Expression_True* S_CreateExpressionTrue(S_Interpreter* interpreter)
{
    DCHECK(interpreter != 0);

    struct S_Expression_True* e = 
        (struct S_Expression_True*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Expression_True));
    DCHECK(e != 0);
    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_TRUE);
    return e;
}

struct S_Expression_False* S_CreateExpressionFalse(S_Interpreter* interpreter)
{
    DCHECK(interpreter != 0);

    struct S_Expression_False* e = 
        (struct S_Expression_False*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Expression_False));
    DCHECK(e != 0);
    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_FALSE);
    return e;
}


struct S_Expression_String* S_CreateExpressionString(struct S_Interpreter* interpreter, char* string)
{
    DCHECK(string != 0);

    struct S_Expression_String* e =
        (struct S_Expression_String*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                        sizeof(struct S_Expression_String));
    DCHECK(e != 0);

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_STRING);
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

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_SYMBOL);
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
    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_NEGATION);
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

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_FUNCTION_CALL);
    e->fn = fn;
    e->param = param;

    return e;
}

struct S_Expression_Function_Define* S_CreateExpressionFunctionDefine(struct S_Interpreter* interpreter,
                                                                      struct S_Parameter_List* param,
                                                                      struct S_Code_Block* code)
{
    DCHECK(interpreter != 0);
    DCHECK(param != 0);
    DCHECK(code != 0);

    struct S_Expression_Function_Define* e = 
        (struct S_Expression_Function_Define*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                                 sizeof(struct S_Expression_Function_Define));
    DCHECK(e != 0);

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_FUNCTION_DEFINE);
    e->param = param;
    e->code  = code;
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

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_OP2);
    e->op = op;
    e->exp1 = exp1;
    e->exp2 = exp2;
    return e;
}

struct S_Expression_Array* S_CreateExpressionArray(struct S_Interpreter* interpreter,
                                                   struct S_Expression_List* exp_list)
{
    DCHECK(interpreter != 0);
    DCHECK(exp_list != 0);

    struct S_Expression_Array *e = 
        (struct S_Expression_Array*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                       sizeof(struct S_Expression_Array));
    DCHECK(e != 0);
    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_ARRAY);
    e->exp_list = exp_list;
    return e;
}

struct S_Expression_Subscript* S_CreateExpressionSubscript(struct S_Interpreter* interpreter,
                                                           struct S_Expression* instance,
                                                           struct S_Expression* index)
{
    DCHECK(interpreter != 0);
    DCHECK(instance != 0);
    DCHECK(index != 0);

    struct S_Expression_Subscript *e = 
        (struct S_Expression_Subscript*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                           sizeof(struct S_Expression_Subscript));
    DCHECK(e != 0);

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_SUBSCRIPT);
    e->instance = instance;
    e->index = index;
    return e;
}

struct S_Expression_Dot* S_CreateExpressionDot(struct S_Interpreter* interpreter,
                                               struct S_Expression* instance,
                                               struct S_Expression_Symbol* field)
{
    DCHECK(interpreter != 0);
    DCHECK(instance != 0);
    DCHECK(field != 0);

    struct S_Expression_Dot *e = 
        (struct S_Expression_Dot*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Expression_Dot));
    DCHECK(e != 0);

    InitializeExpressionHeader(interpreter, (struct S_Expression*)e, EXPRESSION_TYPE_DOT);
    e->instance = instance;
    e->field = field;
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

    param_list->next = 0;
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
    next->next = 0;
    next->symbol = param;

    // push back the new node.
    while (param_list->next != 0)
        param_list = param_list->next;

    param_list->next = next;
}

//// End of Parameter List ////

//// Statement ////
struct S_Statement_Expression* S_CreateStatementExpression(struct S_Interpreter* interpreter, 
                                                           struct S_Expression* expression)
{
    DCHECK(interpreter != 0);
    DCHECK(expression != 0);

    struct S_Statement_Expression* s = 
        (struct S_Statement_Expression*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                           sizeof(struct S_Statement_Expression));
    DCHECK(s != 0);

    s->header.type = STATEMENT_TYPE_EXPRESSION;
    s->exp = expression;
    return s; 
}

struct S_Statement_Global* S_CreateStatementGlobal(struct S_Interpreter* interpreter,
                                                   struct S_Expression_Symbol* symbol)
{
    DCHECK(interpreter != 0);
    DCHECK(symbol != 0);

    struct S_Statement_Global* s = 
        (struct S_Statement_Global*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                       sizeof(struct S_Statement_Global));
    DCHECK(s != 0);

    s->header.type = STATEMENT_TYPE_GLOBAL;
    s->symbol = symbol;
    return s; 
}

struct S_Statement_Return* S_CreateStatementReturn(struct S_Interpreter* interpreter, 
                                                   struct S_Expression* expression)
{
    DCHECK(interpreter != 0);
    DCHECK(expression != 0);

    struct S_Statement_Return* s = 
        (struct S_Statement_Return*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                       sizeof(struct S_Statement_Return));
    DCHECK(s != 0);

    s->header.type = STATEMENT_TYPE_RETURN;
    s->exp = expression;
    return s; 
}

struct S_Statement_Function_Define* S_CreateStatementFunctionDefine(struct S_Interpreter* interpreter,
                                                                    struct S_Expression_Symbol* name,
                                                                    struct S_Parameter_List* param,
                                                                    struct S_Code_Block* code)
{
    DCHECK(interpreter != 0);
    DCHECK(name != 0);
    DCHECK(param != 0);
    DCHECK(code != 0);

    struct S_Statement_Function_Define* s = 
        (struct S_Statement_Function_Define*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                                sizeof(struct S_Statement_Function_Define));
    DCHECK(s != 0);
    s->header.type = STATEMENT_TYPE_FUNCTION_DEFINE;
    s->name = name;
    s->param = param;
    s->code = code;
    return s;
}

struct S_Statement_While* S_CreateStatementWhile(struct S_Interpreter* interpreter,
                                                 struct S_Expression* condition,
                                                 struct S_Code_Block* body)
{
    DCHECK(interpreter != 0);
    DCHECK(condition != 0);
    DCHECK(body != 0);

    struct S_Statement_While* s = 
        (struct S_Statement_While*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                      sizeof(struct S_Statement_While));
    s->header.type = STATEMENT_TYPE_WHILE;
    s->condition = condition;
    s->body = body;
    return s;
}

struct S_Statement_If* S_CreateStatementIf(struct S_Interpreter* interpreter,
                                           struct S_Expression* condition,
                                           struct S_Code_Block* body,
                                           struct S_Elif_List*  elif_list,
                                           struct S_Code_Block* else_body)
{
    DCHECK(interpreter != 0);
    DCHECK(condition != 0);
    DCHECK(body != 0);

    struct S_Statement_If* s = 
        (struct S_Statement_If*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                   sizeof(struct S_Statement_If));
    DCHECK(s != 0);

    s->header.type = STATEMENT_TYPE_IF;
    s->condition = condition;
    s->body = body;
    s->elif_list = elif_list;
    s->else_body = else_body;
    return s;
}
//// End of Statement ////

//// Statement List ////
struct S_Statement_List* S_CreateStatementList(struct S_Interpreter* interpreter, 
                                               struct S_Statement* statement)
{
    DCHECK(interpreter != 0);

    struct S_Statement_List* stat_list = 
        (struct S_Statement_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Statement_List));
    DCHECK(stat_list != 0);

    stat_list->next = 0;
    stat_list->stat = statement;
    return stat_list;
}

void S_AddStatementToStatList(struct S_Interpreter* interpreter,
                              struct S_Statement_List* stat_list,
                              struct S_Statement* statement)
{
    DCHECK(interpreter != 0);
    DCHECK(stat_list != 0);
    DCHECK(statement != 0);

    if (stat_list->stat == 0)
    {
        DCHECK(stat_list->next == 0);
        stat_list->stat = statement;
        return;
    }

    struct S_Statement_List* next = 
        (struct S_Statement_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                     sizeof(struct S_Statement_List));
    DCHECK(next != 0);

    next->next = 0;
    next->stat = statement;

    while (stat_list->next != 0)
        stat_list = stat_list->next;

    stat_list->next = next;
}

//// End of Statement List ////

//// Code Block ////

struct S_Code_Block* S_CreateCodeBlock(struct S_Interpreter* interpreter,
                                       struct S_Statement_List* stat_list)
{
    DCHECK(interpreter != 0);
    
    struct S_Code_Block* code_block = 
        (struct S_Code_Block*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                 sizeof(struct S_Code_Block));
    DCHECK(code_block != 0);

    if (stat_list == 0)
    {
        code_block->stat_list = S_CreateStatementList(interpreter, 0);
    }
    else
    {
        code_block->stat_list = stat_list;
    }

    return code_block;
}
//// End of Clode Block ////

//// Elif List ////
struct S_Elif_List* S_CreateElifList(struct S_Interpreter* interpreter,
                                     struct S_Expression* condition,
                                      struct S_Code_Block* body)
{
    DCHECK(interpreter != 0);
    DCHECK(condition != 0);
    DCHECK(body != 0);

    struct S_Elif_List* elif_list = 
        (struct S_Elif_List*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                sizeof(struct S_Elif_List));
    DCHECK(elif_list != 0);

    elif_list->next = 0;
    elif_list->condition = condition;
    elif_list->body = body;

    return elif_list;
}

void S_AddElifList(struct S_Interpreter* interpreter,
                   struct S_Elif_List* elif_list,
                   struct S_Expression* condition,
                   struct S_Code_Block* body)
{
    DCHECK(interpreter != 0);
    DCHECK(elif_list != 0);
    DCHECK(condition != 0);
    DCHECK(body != 0);

    // append new node to the end.
    struct S_Elif_List* last_node = elif_list;
    while (last_node->next != 0)
        last_node = last_node->next;

    struct S_Elif_List* new_node = S_CreateElifList(interpreter,
                                                    condition,
                                                    body);
    last_node->next = new_node;
}
//// End of Elif List ////