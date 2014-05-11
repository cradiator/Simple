#include "Common.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "S_Expression.h"
#include "S_Context.h"

#include "MM.h"
#include "DBG.h"
#include "ERR.h"

struct S_Value* S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp);
bool S_Eval_Statement_List(struct S_Interpreter* interpreter, struct S_Statement_List* stat_list);
bool S_Eval_Code_Block(struct S_Interpreter* interpreter, struct S_Code_Block* code_block);

struct S_Value* CreateBoolValueHelper(struct S_Interpreter* interpreter, bool true_false)
{
    if (true_false == true)
    {
        return (struct S_Value*)S_CreateValueTrue(interpreter);
    }
    else
    {
        return (struct S_Value*)S_CreateValueFalse(interpreter);
    }
}

/// Evaluate expression ///

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

struct S_Value* S_Eval_Expression_Function_Define(struct S_Interpreter* interpreter, struct S_Expression_Function_Define* exp)
{
    return (S_Value*)S_CreateValueFunction(interpreter, exp->param, exp->code);
}

struct S_Value* S_Eval_Expression_Symbol(struct S_Interpreter* interpreter, struct S_Expression_Symbol* exp)
{
    bool only_local = !S_ContextIsGlobalVar(interpreter, exp->symbol);
    S_Local_Variables* var = S_ContextFindVariable(interpreter, exp->symbol, only_local, false);
	if (var != 0 && var->value != 0)
    {
        return var->value;
    }

    // value not found. mark error.
	ERR_Print(ERR_LEVEL_ERROR, "Line %d: symbol %s is not initialized.", exp->header.lineno, exp->symbol);
    return 0;
}

struct S_Value* S_Eval_Expression_Negation(struct S_Interpreter* interpreter, struct S_Expression_Negation* exp)
{
    struct S_Value* value = S_Eval_Expression(interpreter, exp->exp);
    if (value == 0)
        return 0;

    struct S_Value* returned_value = 0;
    if (value->header.type == VALUE_TYPE_INTEGER)
    { 
        returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, -(((struct S_Value_Integer*)value)->value));
    }
    else if (value->header.type == VALUE_TYPE_DOUBLE)
    {
        returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, -(((struct S_Value_Double*)value)->value));
    }
    else
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s can not negate",
                  exp->header.lineno,
                  VALUE_NAME[value->header.type]);
        returned_value = 0;
    }

    S_MarkValueCollectable(interpreter, value);
    return returned_value;
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
            returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, left_double - right_double);
        else if (op == OP2_MUL)
            returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, left_double * right_double);
        else if (op == OP2_DIV)
            returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, left_double / right_double);
    }

    return returned_value;

__ERROR_DIV_BY_ZERO:
    ERR_Print(ERR_LEVEL_ERROR,
              "Line %d: divided by zero.",
              lineno);
    return 0;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Line %d: %s can not sub/mul/div %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return 0;
}

struct S_Value* EvalFactor(struct S_Interpreter* interpreter, int lineno, struct S_Value* left, struct S_Value* right)
{
    if (left->header.type != VALUE_TYPE_INTEGER && left->header.type != VALUE_TYPE_DOUBLE)
    {
        goto __ERROR_EXIT;
    }

    if (right->header.type != VALUE_TYPE_INTEGER)
    {
        goto __ERROR_EXIT;
    }

    struct S_Value* returned_value = 0;
    struct S_Value_Integer* right_integer = (struct S_Value_Integer*)right;

    // for integer left hand.
    if (left->header.type == VALUE_TYPE_INTEGER)
    {
        struct S_Value_Integer* left_integer = (struct S_Value_Integer*)left;

        // for zero right hand.
        if (right_integer->value == 0)
        {
            if (left_integer->value == 0)
            {
                ERR_Print(ERR_LEVEL_ERROR,
                          "Line: %d 0 can not be factored by 0",
                          lineno);
                return 0;
            }

            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, 0);
            return returned_value;
        }

        // positive right hand.
        if (right_integer->value > 0)
        {
            int value = 1;
            for(int i = 0; i < right_integer->value; ++i)
            {
                value *= left_integer->value;
            }
            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, value);
            return returned_value;
        }

        // negetive right hand.
        double value = 1.0;
        for(int i = 0; i > right_integer->value; --i)
        {
            value /= (double)left_integer->value;
        }
        returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, value);
        return returned_value;
    }
    // for double left hand.
    else
    {
        struct S_Value_Double* left_double = (struct S_Value_Double*)left;
        // for zero right hand.
        if (right_integer->value == 0)
        {
            if (left_double->value == 0.0)
            {
                ERR_Print(ERR_LEVEL_ERROR,
                          "Line: %d 0.0 can not be factored by 0",
                          lineno);
                return 0;
            }

            returned_value = (struct S_Value*)S_CreateValueInteger(interpreter, 0);
            return returned_value;
        }

        // positive right hand.
        if (right_integer->value > 0)
        {
            double value = 1.0;
            for(int i = 0; i < right_integer->value; ++i)
            {
                value *= left_double->value;
            }
            returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, value);
            return returned_value;
        }

        // negetive right hand.
        double value = 1.0;
        for(int i = 0; i > right_integer->value; --i)
        {
            value /= (double)left_double->value;
        }
        returned_value = (struct S_Value*)S_CreateValueDouble(interpreter, value);
        return returned_value;
    }

    // SHOUDN NOT GO THERE.
    DCHECK(false);


__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Line %d: %s can not factor %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return 0;
}

struct S_Value* EvalCmp(struct S_Interpreter* interpreter, int lineno, struct S_Value* left, struct S_Value* right, int op)
{
    DCHECK(op >= OP2_G && op <= OP2_NE);

    struct S_Value* returned_value = 0;
    int cmp_result = 0;

    // string compare.
    if (left->header.type == VALUE_TYPE_STRING)
    {
        if (right->header.type != VALUE_TYPE_STRING)
        {
            goto __ERROR_EXIT;
        }

        struct S_Value_String* left_string  = (struct S_Value_String*)left;
        struct S_Value_String* right_string = (struct S_Value_String*)right;
        int cmp_result = strcmp(left_string->string, right_string->string);
    }
    else if (left->header.type == VALUE_TYPE_INTEGER && right->header.type == VALUE_TYPE_INTEGER)
    {
        struct S_Value_Integer* left_integer  = (struct S_Value_Integer*)left;
        struct S_Value_Integer* right_integer = (struct S_Value_Integer*)right;

        int cmp_result = left_integer->value - right_integer->value;
    }
    // number compare
    else if (left->header.type == VALUE_TYPE_DOUBLE || right->header.type == VALUE_TYPE_DOUBLE)
    {
        double left_double  = 0.0;
        double right_double = 0.0;

        if (left->header.type == VALUE_TYPE_DOUBLE)
        {
            left_double = ((struct S_Value_Double*)left)->value;
        }
        else if (left->header.type == VALUE_TYPE_INTEGER)
        {
            left_double = ((struct S_Value_Integer*)left)->value;
        }
        else
        {
            goto __ERROR_EXIT;
        }

        if (right->header.type == VALUE_TYPE_DOUBLE)
        {
            right_double = ((struct S_Value_Double*)right)->value;
        }
        else if (right->header.type == VALUE_TYPE_INTEGER)
        {
            right_double = ((struct S_Value_Integer*)right)->value;
        }
        else
        {
            goto __ERROR_EXIT;
        }

        double result = left_double - right_double;
        if (result > 0)
            cmp_result = 1;
        else if (result == 0)
            cmp_result = 0;
        else
            cmp_result = -1;
    }
    // true/false compare
    else if (left->header.type == VALUE_TYPE_TRUE || left->header.type == VALUE_TYPE_FALSE)
    {
        if (right->header.type != VALUE_TYPE_TRUE && right->header.type != VALUE_TYPE_FALSE)
        {
            goto __ERROR_EXIT;
        }

        if (op != OP2_E && op != OP2_NE)
        {
            goto __ERROR_EXIT;
        }

        cmp_result = left->header.type - right->header.type;
    }
    else
    {
        goto __ERROR_EXIT;
    }

    bool true_false = false;
    if (op == OP2_G)
    {
        true_false = cmp_result > 0 ? true : false; 
    }
    else if (op == OP2_GE)
    {
        true_false = cmp_result >= 0 ? true : false; 
    }
    else if (op == OP2_E)
    {
        true_false = cmp_result == 0 ? true : false; 
    }
    else if (op == OP2_L)
    {
        true_false = cmp_result < 0 ? true : false; 
    }
    else if (op == OP2_LE)
    {
        true_false = cmp_result <= 0 ? true : false; 
    }
    else if (op == OP2_NE)
    {
        true_false = cmp_result != 0 ? true : false; 
    }

    returned_value = CreateBoolValueHelper(interpreter, true_false);
    return returned_value;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR,
              "Line %d: %s can not compare with %s, opcode: %d",
              lineno,
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type],
              op);
    return 0;
}

struct S_Value* EvalRel(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
{
    DCHECK(exp->op == OP2_AND || exp->op == OP2_OR);

    struct S_Value* left  = 0;
    struct S_Value* right = 0;
    struct S_Value* result_value = 0;
    if (exp->op == OP2_AND)
    {
        left = S_Eval_Expression(interpreter, exp->exp1);
        if (left == 0)
        {
            goto __EXIT;
        }

        if (left->header.type == VALUE_TYPE_FALSE || left->header.type == VALUE_TYPE_NIL)
        {
            result_value = CreateBoolValueHelper(interpreter, false);
        }
        else if (left->header.type != VALUE_TYPE_TRUE)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not && something",
                      exp->header.lineno,
                      VALUE_NAME[left->header.type]);
            goto __EXIT;
        }

        // left is true.
        right = S_Eval_Expression(interpreter, exp->exp2);
        if (right == 0)
        {
            goto __EXIT;
        }
        else if (right->header.type == VALUE_TYPE_FALSE || right->header.type == VALUE_TYPE_NIL)
        {
            result_value = CreateBoolValueHelper(interpreter, false);
        }
        else if (right->header.type == VALUE_TYPE_TRUE)
        {
            result_value = CreateBoolValueHelper(interpreter, true);
        }
        else
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not be && with",
                      exp->header.lineno,
                      VALUE_NAME[right->header.type]);
            goto __EXIT;
        }
    }
    else
    {
        left = S_Eval_Expression(interpreter, exp->exp1);
        if (left == 0)
        {
            goto __EXIT;
        }

        if (left->header.type == VALUE_TYPE_TRUE)
        {
            result_value = CreateBoolValueHelper(interpreter, true);
        }
        else if (left->header.type != VALUE_TYPE_FALSE && left->header.type != VALUE_TYPE_NIL)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not || something",
                      exp->header.lineno,
                      VALUE_NAME[left->header.type]);
            goto __EXIT;
        }

        // left is false or nil.
        right = S_Eval_Expression(interpreter, exp->exp2);
        if (right == 0)
        {
            goto __EXIT;
        }
        else if (right->header.type == VALUE_TYPE_FALSE || right->header.type == VALUE_TYPE_NIL)
        {
            result_value = CreateBoolValueHelper(interpreter, false);
        }
        else if (right->header.type == VALUE_TYPE_TRUE)
        {
            result_value = CreateBoolValueHelper(interpreter, true);
        }
        else
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not be || with",
                      exp->header.lineno,
                      VALUE_NAME[right->header.type]);
            goto __EXIT;
        }
    }

__EXIT:
    if (left != 0)
        S_MarkValueCollectable(interpreter, left);
    if (right != 0)
        S_MarkValueCollectable(interpreter, right);
    return 0;
}

struct S_Value* EvalAssign(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
{
    DCHECK(exp->op == OP2_ASSIGN);

    if (exp->exp1->header.type != EXPRESSION_TYPE_SYMBOL)
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s is not an symbol, can not be assigned.",
                  exp->header.lineno,
                  VALUE_NAME[exp->exp1->header.type]);
    }

    // Eval right hands.
    struct S_Expression_Symbol* left_symbol = (struct S_Expression_Symbol*)exp->exp1;
    struct S_Value* right_value = S_Eval_Expression(interpreter, exp->exp2);
    if (right_value == 0)
        return 0;

    // Create or Get context variables.
    bool is_symbol_global = S_ContextIsGlobalVar(interpreter, left_symbol->symbol);
    struct S_Local_Variables* variables = S_ContextFindVariable(interpreter, left_symbol->symbol, !is_symbol_global, true);
    DCHECK(variables != 0);

    // Assign the value.
    variables->value = right_value;
    S_MarkValueCollectable(interpreter, right_value);

    return right_value;
}

struct S_Value* S_Eval_Expression_Op2(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
{
    // In exp1 && exp2, we should not evaluate exp2 unless exp1 is true
    // In exp1 || exp2, we should not evaluate exp2 unless exp1 is false.
    // So Relationship need special treatment.
    if (exp->op == OP2_AND || exp->op == OP2_OR)
    {
        return EvalRel(interpreter, exp);
    }

    // Assign operation is different from other two hands op.
    if (exp->op == OP2_ASSIGN)
    {
        return EvalAssign(interpreter, exp);
    }

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

        case OP2_FACTOR:
            returned_value = EvalFactor(interpreter, exp->header.lineno, left, right);
            break;

        case OP2_G:
        case OP2_GE:
        case OP2_E:
        case OP2_L:
        case OP2_LE:
        case OP2_NE:
            returned_value = EvalCmp(interpreter, exp->header.lineno, left, right, exp->op);
            break;
            
        default:
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: invalid operator type %d.",
                      exp->header.lineno,
                      exp->op);
            DCHECK(false);
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

    case EXPRESSION_TYPE_NEGATION:
        returned_value = S_Eval_Expression_Negation(interpreter, (struct S_Expression_Negation*)exp);
        break;

    case EXPRESSION_TYPE_OP2:
        returned_value = S_Eval_Expression_Op2(interpreter, (struct S_Expression_Op2*)exp);
        break;

    case EXPRESSION_TYPE_FUNCTION_DEFINE:
        returned_value = S_Eval_Expression_Function_Define(interpreter, (struct S_Expression_Function_Define*)exp);
        break;

    default:
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: expression type %d is unsupport",
                  exp->header.lineno,
                  exp->header.type);
        DCHECK(false);
        break;

    }

    return returned_value;
}

/// End of Evaluate expression ///

/// Evaluate statement ///
bool S_Eval_Statement_Expression(struct S_Interpreter* interpreter, struct S_Statement_Expression* stat)
{
    struct S_Value* returned_value = S_Eval_Expression(interpreter, stat->exp);
    if (returned_value == 0)
        return false;

    S_MarkValueCollectable(interpreter, returned_value);
    return true;
}

bool S_Eval_Statement_Global(struct S_Interpreter* interpreter, struct S_Statement_Global* stat)
{
    struct S_Expression_Symbol* symbol = stat->symbol;
    S_ContextMarkVarGlobal(interpreter, symbol->symbol);
    return true;
}

bool S_Eval_Statement_Return(struct S_Interpreter* interpreter, struct S_Statement_Return* stat)
{
    struct S_Value* returned_value = S_Eval_Expression(interpreter, stat->exp);
    if (returned_value == 0)
        return false;

    S_SetReturnValue(interpreter, returned_value);
    return true;
}

bool S_Eval_Statement_Function_Define(struct S_Interpreter* interpreter, struct S_Statement_Function_Define* stat)
{
    // Find or Create a local variable.
    struct S_Local_Variables* variable = S_ContextFindVariable(interpreter, stat->name->symbol, true, true);
    if (variable == 0)
        return false;

    struct S_Value_Function* function = S_CreateValueFunction(interpreter, stat->param, stat->code);
    DCHECK(function != 0);
    variable->value = (struct S_Value*)function;

    S_MarkValueCollectable(interpreter, (struct S_Value*)function);
    return true;
}

bool S_Eval_Statement_While(struct S_Interpreter* interpreter, struct S_Statement_While* stat)
{
    bool success = false;
    struct S_Value* condition_result = 0;
    for (;;)
    {
        // evaluate conditon expression, return if failed.
        condition_result = S_Eval_Expression(interpreter, stat->condition);
        if (condition_result == 0)
        {
            success = false;
            break;
        }

        if (condition_result->header.type == VALUE_TYPE_TRUE)
        {
            S_MarkValueCollectable(interpreter, condition_result);
            success = S_Eval_Code_Block(interpreter, stat->body);
            if (!success)
                break;

            // if success, loop back.
        }
        // condition is false, return.
        else if (condition_result->header.type == VALUE_TYPE_FALSE || condition_result->header.type == VALUE_TYPE_NIL)
        {
            S_MarkValueCollectable(interpreter, condition_result);
            success = true;
            break;
        }
        else
        {
            S_MarkValueCollectable(interpreter, condition_result);
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: %s can not be a condition.",
                      stat->condition->header.lineno,
                      VALUE_NAME[condition_result->header.type]);
            success = false;
            break;
        }
    }

    return success;
}

bool S_Eval_Statement_If(struct S_Interpreter* interpreter, struct S_Statement_If* stat)
{
    struct S_Value* condition_result = S_Eval_Expression(interpreter, stat->condition);
    bool success = false;

    if (condition_result->header.type == VALUE_TYPE_TRUE)
    {
        S_MarkValueCollectable(interpreter, condition_result);
        success = S_Eval_Code_Block(interpreter, stat->body);
    }
    else if (condition_result->header.type == VALUE_TYPE_FALSE || condition_result->header.type == VALUE_TYPE_NIL)
    {
        S_MarkValueCollectable(interpreter, condition_result);
        if (stat->else_body != 0)
            success = S_Eval_Code_Block(interpreter, stat->else_body);
        else
            success = true;
    }
    else
    {
        S_MarkValueCollectable(interpreter, condition_result);
        success = false;
        ERR_Print(ERR_LEVEL_ERROR,
                 "Line %d: %s can not be a condition.",
                 stat->condition->header.lineno,
                 VALUE_NAME[condition_result->header.type]);
    }

    return success;
}

bool S_Eval_Statement(struct S_Interpreter* interpreter, struct S_Statement* stat)
{
    DCHECK(interpreter != 0);

    DCHECK(stat != 0);

    bool eval_success = false;
    switch(stat->header.type)
    {
        case STATEMENT_TYPE_EXPRESSION:
            eval_success = S_Eval_Statement_Expression(interpreter, (struct S_Statement_Expression*)stat);
            break;

        case STATEMENT_TYPE_GLOBAL:
            eval_success = S_Eval_Statement_Global(interpreter, (struct S_Statement_Global*)stat);
            break;

        case STATEMENT_TYPE_RETURN:
            eval_success = S_Eval_Statement_Return(interpreter, (struct S_Statement_Return*)stat);
            break;

        case STATEMENT_TYPE_FUNCTION_DEFINE:
            eval_success = S_Eval_Statement_Function_Define(interpreter, (struct S_Statement_Function_Define*)stat);
            break;

        case STATEMENT_TYPE_WHILE:
            eval_success = S_Eval_Statement_While(interpreter, (struct S_Statement_While*)stat);
            break;

        case STATEMENT_TYPE_IF:
            eval_success = S_Eval_Statement_If(interpreter, (struct S_Statement_If*)stat);
            break;
            
        default:
            ERR_Print(ERR_LEVEL_ERROR,
                      "Invalid statement type. %d",
                      stat->header.type);
        DCHECK(false);
        break;
    }

    return eval_success;
}

/// End of Evaluate statement


/// Statement list ///
bool S_Eval_Statement_List(struct S_Interpreter* interpreter, struct S_Statement_List* stat_list)
{
    DCHECK(interpreter != 0);
    DCHECK(stat_list != 0);

    // empty list, return true.
    if (stat_list == 0)
    {
        DCHECK(stat_list->next == 0);
        return true;
    }

    bool success = true;
    
    struct S_Statement_List* current_node = stat_list;
    while (current_node != 0 && success)
    {
        // evaluate current node. if error, return
        struct S_Statement* stat = current_node->stat;
        success = S_Eval_Statement(interpreter, stat);
        if (!success)
            break;

        // have return, break the loop
        if (S_IsHaveReturnValue(interpreter))
        {
            DCHECK(stat->header.type == STATEMENT_TYPE_RETURN);
            break;
        }
    }

    return success;
}
/// End of statement list ///

/// Code Block ///
bool S_Eval_Code_Block(struct S_Interpreter* interpreter, struct S_Code_Block* code_block)
{
    if (code_block->stat_list == 0)
    {
        return true;
    }

    return S_Eval_Statement_List(interpreter, code_block->stat_list);
}

/// End of Code Block ///

