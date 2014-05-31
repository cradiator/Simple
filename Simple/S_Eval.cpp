#include "Common.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "S_Expression.h"
#include "S_Context.h"

#include "MM.h"
#include "DBG.h"
#include "ERR.h"

bool S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp);
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

bool S_Eval_Expression_Nil(struct S_Interpreter* interpreter, struct S_Expression_Nil* exp)
{
    struct S_Value* v = (struct S_Value*)S_CreateValueNil(interpreter);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_Integer(struct S_Interpreter* interpreter, struct S_Expression_Integer* exp)
{
    struct S_Value* v =  (struct S_Value*)S_CreateValueInteger(interpreter, exp->value);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_Double(struct S_Interpreter* interpreter, struct S_Expression_Double* exp)
{
    struct S_Value* v =  (struct S_Value*)S_CreateValueDouble(interpreter, exp->value);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_True(struct S_Interpreter* interpreter, struct S_Expression_True* exp)
{
    struct S_Value* v = (struct S_Value*)S_CreateValueTrue(interpreter);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_False(struct S_Interpreter* interpreter, struct S_Expression_False* exp)
{
    struct S_Value* v = (struct S_Value*)S_CreateValueFalse(interpreter);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_String(struct S_Interpreter* interpreter, struct S_Expression_String* exp)
{
	struct S_Value* v = (struct S_Value*)S_CreateValueString(interpreter, exp->string);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_Function_Define(struct S_Interpreter* interpreter, struct S_Expression_Function_Define* exp)
{
    struct S_Value* v = (S_Value*)S_CreateValueFunction(interpreter, exp->param, exp->code);
    S_PushRuntimeStackValue(interpreter, v);
    return true;
}

bool S_Eval_Expression_Symbol(struct S_Interpreter* interpreter, struct S_Expression_Symbol* exp)
{
    bool only_local = !S_ContextIsGlobalVar(interpreter, exp->symbol);
    S_Local_Variables* var = S_ContextFindVariable(interpreter, exp->symbol, only_local, false);
	if (var != 0 && var->value != 0)
    {
        S_PushRuntimeStackValue(interpreter, var->value);
        return true;
    }

    // value not found. mark error.
	ERR_Print(ERR_LEVEL_ERROR, "Line %d: symbol %s is not initialized.", exp->header.lineno, exp->symbol);
    return false;
}

bool S_Eval_Expression_Negation(struct S_Interpreter* interpreter, struct S_Expression_Negation* exp)
{
    bool success = S_Eval_Expression(interpreter, exp->exp);
    if (!success)
        return false;

    struct S_Value* value  = S_PopRuntimeStackValue(interpreter);
    struct S_Value* result = 0;
    if (value->header.type == VALUE_TYPE_INTEGER)
    { 
        result = (struct S_Value*)S_CreateValueInteger(interpreter, -(((struct S_Value_Integer*)value)->value));
        S_PushRuntimeStackValue(interpreter, result);
    }
    else if (value->header.type == VALUE_TYPE_DOUBLE)
    {
        result = (struct S_Value*)S_CreateValueDouble(interpreter, -(((struct S_Value_Double*)value)->value));
        S_PushRuntimeStackValue(interpreter, result);
    }
    else
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s can not negate",
                  exp->header.lineno,
                  VALUE_NAME[value->header.type]);
    }

    return success;
}

bool EvalAdd(struct S_Interpreter* interpreter, int lineno)
{
    struct S_Value* left  = S_PopRuntimeStackValue(interpreter);
    struct S_Value* right = S_PopRuntimeStackValue(interpreter);

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

        S_PushRuntimeStackValue(interpreter, returned_value);
        return true;        
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

    S_PushRuntimeStackValue(interpreter, returned_value);
    return true;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Lind %d: %s can not add %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return false;
}

bool EvalSubMulDiv(struct S_Interpreter* interpreter, int lineno, int op)
{
    DCHECK(op == OP2_SUB || op == OP2_MUL || op == OP2_DIV);

    struct S_Value* left  = S_PopRuntimeStackValue(interpreter);
    struct S_Value* right = S_PopRuntimeStackValue(interpreter);

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

    S_PushRuntimeStackValue(interpreter, returned_value);
    return true;

__ERROR_DIV_BY_ZERO:
    ERR_Print(ERR_LEVEL_ERROR,
              "Line %d: divided by zero.",
              lineno);
    return false;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Line %d: %s can not sub/mul/div %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return false;
}

bool EvalFactor(struct S_Interpreter* interpreter, int lineno)
{
    struct S_Value* left  = S_PopRuntimeStackValue(interpreter);
    struct S_Value* right = S_PopRuntimeStackValue(interpreter);
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
                return false;
            }

            S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueInteger(interpreter, 0));
            return true;
        }

        // positive right hand.
        if (right_integer->value > 0)
        {
            int value = 1;
            for(int i = 0; i < right_integer->value; ++i)
            {
                value *= left_integer->value;
            }
            S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueInteger(interpreter, value));
            return true;
        }

        // negetive right hand.
        double value = 1.0;
        for(int i = 0; i > right_integer->value; --i)
        {
            value /= (double)left_integer->value;
        }
        S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueDouble(interpreter, value));
        return true;
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
                return false;
            }

            S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueInteger(interpreter, 0));
            return true;
        }

        // positive right hand.
        if (right_integer->value > 0)
        {
            double value = 1.0;
            for(int i = 0; i < right_integer->value; ++i)
            {
                value *= left_double->value;
            }
            S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueDouble(interpreter, value));
            return true;
        }

        // negetive right hand.
        double value = 1.0;
        for(int i = 0; i > right_integer->value; --i)
        {
            value /= (double)left_double->value;
        }
        S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueDouble(interpreter, value));
        return true;
    }

    // SHOUDN NOT GO THERE.
    DCHECK(false);


__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR, 
              "Line %d: %s can not factor %s.", 
              lineno, 
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type]);
    return false;
}

bool EvalCmp(struct S_Interpreter* interpreter, int lineno, int op)
{
    DCHECK(op >= OP2_G && op <= OP2_NE);

    struct S_Value* returned_value = 0;
    int cmp_result = 0;

    struct S_Value* left  = S_PopRuntimeStackValue(interpreter);
    struct S_Value* right = S_PopRuntimeStackValue(interpreter);

    // string compare.
    if (left->header.type == VALUE_TYPE_STRING)
    {
        if (right->header.type != VALUE_TYPE_STRING)
        {
            goto __ERROR_EXIT;
        }

        struct S_Value_String* left_string  = (struct S_Value_String*)left;
        struct S_Value_String* right_string = (struct S_Value_String*)right;
        cmp_result = strcmp(left_string->string, right_string->string);
    }
    else if (left->header.type == VALUE_TYPE_INTEGER && right->header.type == VALUE_TYPE_INTEGER)
    {
        struct S_Value_Integer* left_integer  = (struct S_Value_Integer*)left;
        struct S_Value_Integer* right_integer = (struct S_Value_Integer*)right;

        cmp_result = left_integer->value - right_integer->value;
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

    S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, true_false));
    return true;

__ERROR_EXIT:
    ERR_Print(ERR_LEVEL_ERROR,
              "Line %d: %s can not compare with %s, opcode: %d",
              lineno,
              VALUE_NAME[left->header.type],
              VALUE_NAME[right->header.type],
              op);
    return false;
}

bool EvalRel(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
{
    DCHECK(exp->op == OP2_AND || exp->op == OP2_OR);

    bool success = false;
    struct S_Value* temp_value = 0;
    if (exp->op == OP2_AND)
    {
        success = S_Eval_Expression(interpreter, exp->exp1);
        if (!success)
            goto __EXIT;

        temp_value = S_PopRuntimeStackValue(interpreter);

        if (temp_value->header.type == VALUE_TYPE_FALSE || temp_value->header.type == VALUE_TYPE_NIL)
        {
            S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, false));
            success = true;
            goto __EXIT;
        }
        else if (temp_value->header.type != VALUE_TYPE_TRUE)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not && something",
                      exp->header.lineno,
                      VALUE_NAME[temp_value->header.type]);
            success = false;
            goto __EXIT;
        }

        // left is true.
        success = S_Eval_Expression(interpreter, exp->exp2);
        if (!success)
        {
            goto __EXIT;
        }

        temp_value = S_PopRuntimeStackValue(interpreter);
        if (temp_value->header.type == VALUE_TYPE_FALSE || temp_value->header.type == VALUE_TYPE_NIL)
        {
            S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, false));
            success = true;
            goto __EXIT;
        }
        else if (temp_value->header.type == VALUE_TYPE_TRUE)
        {
            S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, true));
            success = true;
            goto __EXIT;
        }
        else
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not be && with",
                      exp->header.lineno,
                      VALUE_NAME[temp_value->header.type]);
            success = false;
            goto __EXIT;
        }
    }
    else
    {
        success = S_Eval_Expression(interpreter, exp->exp1);
        if (!success)
        {
            goto __EXIT;
        }

        temp_value = S_PopRuntimeStackValue(interpreter);
        if (temp_value->header.type == VALUE_TYPE_TRUE)
        {
            S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, true));
            success = true;
            goto __EXIT;
        }
        else if (temp_value->header.type != VALUE_TYPE_FALSE && temp_value->header.type != VALUE_TYPE_NIL)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not || something",
                      exp->header.lineno,
                      VALUE_NAME[temp_value->header.type]);
            success = false;
            goto __EXIT;
        }

        // left is false or nil.
        success = S_Eval_Expression(interpreter, exp->exp2);
        if (!success)
        {
            goto __EXIT;
        }

        temp_value = S_PopRuntimeStackValue(interpreter);
        if (temp_value->header.type == VALUE_TYPE_FALSE || temp_value->header.type == VALUE_TYPE_NIL)
        {
            S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, false));
            success = true;
            goto __EXIT;
        }
        else if (temp_value->header.type == VALUE_TYPE_TRUE)
        {
            S_PushRuntimeStackValue(interpreter, CreateBoolValueHelper(interpreter, true));
            success = true;
            goto __EXIT;
        }
        else
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: type %s can not be || with",
                      exp->header.lineno,
                      VALUE_NAME[temp_value->header.type]);
            success = false;
            goto __EXIT;
        }
    }

__EXIT:
    return success;
}

bool EvalAssign(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
{
    DCHECK(exp->op == OP2_ASSIGN);

    if (exp->exp1->header.type != EXPRESSION_TYPE_SYMBOL && 
        exp->exp1->header.type != EXPRESSION_TYPE_SUBSCRIPT &&
        exp->exp1->header.type != EXPRESSION_TYPE_DOT)
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s is not an symbol or array, can not be assigned.",
                  exp->header.lineno,
                  VALUE_NAME[exp->exp1->header.type]);
        return false;
    }

    // Eval right hands.
    bool success = S_Eval_Expression(interpreter, exp->exp2);
    if (!success)
        return false;

    success = false;
    if (exp->exp1->header.type == EXPRESSION_TYPE_SYMBOL)
    {
        struct S_Expression_Symbol* left_symbol = (struct S_Expression_Symbol*)exp->exp1;

        // Create or Get context variables.
        bool is_symbol_global = S_ContextIsGlobalVar(interpreter, left_symbol->symbol);
        struct S_Local_Variables* variables = S_ContextFindVariable(interpreter, left_symbol->symbol, !is_symbol_global, true);
        DCHECK(variables != 0);

        // Assign the value.
        variables->value = S_PeekRuntimeStackValue(interpreter, 0);
        success = true;
    }
    else if (exp->exp1->header.type == EXPRESSION_TYPE_SUBSCRIPT)
    {
        // subscript array.

        // eval instance
        struct S_Expression_Subscript* exp_sub = (struct S_Expression_Subscript*)exp->exp1;
        success = S_Eval_Expression(interpreter, exp_sub->instance);
        if (!success)
        {
            S_PopRuntimeStackValue(interpreter);    // pop instance
            goto __EXIT;
        }

        struct S_Value_Array* instance = (struct S_Value_Array*)S_PeekRuntimeStackValue(interpreter, 0);
        if (instance->header.type != VALUE_TYPE_ARRAY)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                "Line %d: %s can not be subscripted assignment.",
                exp_sub->instance->header.lineno,
                VALUE_NAME[instance->header.type]);
            S_PopRuntimeStackValue(interpreter);    // pop instance
            S_PopRuntimeStackValue(interpreter);    // pop right hand value
            success = false;
            goto __EXIT;
        }

        // eval index
        if (exp_sub->index->header.type == EXPRESSION_TYPE_RANGE)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: not support range assignment.",
                      exp_sub->index->header.lineno);
            S_PopRuntimeStackValue(interpreter);    // pop instance
            S_PopRuntimeStackValue(interpreter);    // pop right hand value
            success = false;
            goto __EXIT;
        }

        success = S_Eval_Expression(interpreter, exp_sub->index);
        if (!success)
        {
            S_PopRuntimeStackValue(interpreter);    // pop instance
            S_PopRuntimeStackValue(interpreter);    // pop right hand value
            success = false;
            goto __EXIT;
        }

        struct S_Value_Integer* index = (struct S_Value_Integer*)S_PeekRuntimeStackValue(interpreter, 0);
        if (index->header.type != VALUE_TYPE_INTEGER)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                "Line %d: %s can not be an array index.",
                exp_sub->index->header.lineno,
                VALUE_NAME[index->header.type]);
            success = false;
            S_PopRuntimeStackValue(interpreter);    // pop index
            S_PopRuntimeStackValue(interpreter);    // pop instance
            S_PopRuntimeStackValue(interpreter);    // pop right hand value
            goto __EXIT;
        }

        if ((unsigned int)index->value >= instance->array_size)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                "Line %d: Out of index.",
                exp_sub->index->header.lineno);
            success = false;
            S_PopRuntimeStackValue(interpreter);    // pop index
            S_PopRuntimeStackValue(interpreter);    // pop instance
            S_PopRuntimeStackValue(interpreter);    // pop right hand value
            goto __EXIT;
        }

        // assign value
        instance->value_array[index->value] = S_PeekRuntimeStackValue(interpreter, 2); // get right hand value

        S_PopRuntimeStackValue(interpreter);    // pop index
        S_PopRuntimeStackValue(interpreter);    // pop instance
        success = true;
    }
    else if (exp->exp1->header.type == EXPRESSION_TYPE_DOT)
    {
        struct S_Expression_Dot* exp_dot = (struct S_Expression_Dot*)exp->exp1;
        success = S_Eval_Expression(interpreter, exp_dot->instance);
        if (!success)
        {
            goto __EXIT;
        }
        struct S_Value* instance = S_PeekRuntimeStackValue(interpreter, 0);
        struct S_Value* right_hand = S_PeekRuntimeStackValue(interpreter, 1);
        struct S_Field_List* field = S_Find_Value_Field(interpreter, instance, exp_dot->field->symbol, true);
        DCHECK(field != 0);
        field->value = right_hand;

        S_PopRuntimeStackValue(interpreter);    // pop instance out.
        success = true;
    }

__EXIT:
    return success;
}

bool S_Eval_Expression_Op2(struct S_Interpreter* interpreter, struct S_Expression_Op2* exp)
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

    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);

    // Push exp2 and exp1 result.
    bool success = false;
    success = S_Eval_Expression(interpreter, exp->exp2);
    if (!success)
        goto __EXIT;

    success = S_Eval_Expression(interpreter, exp->exp1);
    if (!success)
    {
        S_PopRuntimeStackValue(interpreter);    // pop exp->exp2 result out.
        goto __EXIT;
    }
    
    switch(exp->op)
    {
        case OP2_ADD:
            success = EvalAdd(interpreter, exp->header.lineno);
            break;

        case OP2_SUB:
        case OP2_MUL:
        case OP2_DIV:
            success = EvalSubMulDiv(interpreter, exp->header.lineno, exp->op);
            break;

        case OP2_FACTOR:
            success = EvalFactor(interpreter, exp->header.lineno);
            break;

        case OP2_G:
        case OP2_GE:
        case OP2_E:
        case OP2_L:
        case OP2_LE:
        case OP2_NE:
            success = EvalCmp(interpreter, exp->header.lineno, exp->op);
            break;
            
        default:
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: invalid operator type %d.",
                      exp->header.lineno,
                      exp->op);
            DCHECK(false);
            success = false;
            break;
    }

__EXIT:
    if (!success)
    {
        while (start_stack_index != S_GetRuntimeStackSize(interpreter))
            S_PopRuntimeStackValue(interpreter);
    }

    return success;
}

bool S_Eval_Expression_Function_Call(struct S_Interpreter* interpreter, struct S_Expression_Function_Call* exp)
{
    bool success = false;
    int start_stack_index = S_GetRuntimeStackSize(interpreter);
    struct S_Value* returned_value = 0;
    struct S_Value** value_array = 0;
    bool has_pushed_contenxt = false;

    // Get function pointer.
    if (exp->fn->header.type != EXPRESSION_TYPE_SYMBOL)
    {
        success = S_Eval_Expression(interpreter, exp->fn);
    }
    else
    {
        struct S_Expression_Symbol* func_symbol = (struct S_Expression_Symbol*)exp->fn;
        struct S_Local_Variables* var = S_ContextFindVariable(interpreter, func_symbol->symbol, false, false);  // search global
        if (var == 0)
        {
            success = false;
            ERR_Print(ERR_LEVEL_ERROR,
                "Line %d: Can not find function %s.",
                exp->fn->header.lineno,
                func_symbol->symbol);
            goto __EXIT;
        }

        success = true;
        S_PushRuntimeStackValue(interpreter, var->value);
    }
    
    if (!success)
        return success;

    struct S_Value* function_value = S_PeekRuntimeStackValue(interpreter, 0);

    if (function_value->header.type != VALUE_TYPE_FUNCTION)
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s can not be called as a function.",
                  exp->header.lineno,
                  VALUE_NAME[function_value->header.type]);
        S_PopRuntimeStackValue(interpreter);
        return false;
    }
    struct S_Value_Function* function = (struct S_Value_Function*)function_value;

    // is a class method?
    bool is_method = false;
    struct S_Value* class_instance = 0;
    if (exp->fn->header.type == EXPRESSION_TYPE_DOT)
    {
        is_method = true;
        struct S_Expression_Dot* dot = (struct S_Expression_Dot*)exp->fn;
        success = S_Eval_Expression(interpreter, dot->instance);
        if (!success)
            goto __EXIT;

        class_instance = S_PeekRuntimeStackValue(interpreter, 0);
    }

    // Evaluate the function body.
    if (function->type == NATIVE_FUNCTION)
    {
        struct S_Expression_List* current_exp_list = exp->param;
        S_NativeFunctionProto native_ptr = function->u.native;

        // empty parameter list
        if (current_exp_list == 0 || current_exp_list->exp == 0)
        {
            S_ContextPush(interpreter);
            has_pushed_contenxt = true;
            unsigned int prev_stack_count = S_GetRuntimeStackSize(interpreter);
            if (is_method)
                success = native_ptr(interpreter, &class_instance, 1);
            else
                success = native_ptr(interpreter, 0, 0);
            if (success)
            {
                DCHECK(prev_stack_count + 1 == S_GetRuntimeStackSize(interpreter));
                returned_value = S_PopRuntimeStackValue(interpreter);
            }
            else
            {
                DCHECK(prev_stack_count == S_GetRuntimeStackSize(interpreter));
            }
            goto __EXIT;
        }

        // non-empty paramter list.
        // count paramter
        int exp_count = 0;

        // first element of a method is this pointer
        if (is_method)
            exp_count++;

        while (current_exp_list != 0)
        {
            exp_count++;
            current_exp_list = current_exp_list->next;
        }

        // create value array and evaluate paramter expression.
        value_array = (struct S_Value**)malloc(sizeof(struct S_Value*) * exp_count);
        current_exp_list = exp->param;
        int current_param_index = 0;

        // class method call, the first parameter is this pointer.
        if (is_method)
        {
            value_array[current_param_index] = class_instance;
            current_param_index++;
        }

        while (current_exp_list != 0)
        {
            success = S_Eval_Expression(interpreter, current_exp_list->exp);
            if (!success)
            {
                goto __EXIT;
            }

            value_array[current_param_index] = S_PeekRuntimeStackValue(interpreter, 0);
            current_exp_list = current_exp_list->next;
            current_param_index++;
        }

        // Call native function.
        S_ContextPush(interpreter);
        unsigned int prev_stack_count = S_GetRuntimeStackSize(interpreter);
        has_pushed_contenxt = true;
        success = native_ptr(interpreter, value_array, exp_count);
        if (success)
        {
            DCHECK(prev_stack_count + 1 == S_GetRuntimeStackSize(interpreter));
            returned_value = S_PopRuntimeStackValue(interpreter);
        }
        else
        {
            DCHECK(prev_stack_count == S_GetRuntimeStackSize(interpreter));
        }
    }
    else if (function->type == SCRIPT_FUNCTION)
    {
        // Prepare paremeter.
        struct S_Expression_List* current_exp_list = exp->param;
        struct S_Parameter_List* current_param_list = function->u.script.param_list;

        // count parameter and expression.
        int exp_count = 0;
        while (current_exp_list != 0 && current_exp_list->exp != 0)
        {
            exp_count++;
            current_exp_list = current_exp_list->next;
        }

        int param_count = 0;
        while (current_param_list != 0 && current_param_list->symbol != 0)
        {
            param_count++;
            current_param_list = current_param_list->next;
        }

        if (exp_count != param_count)
            goto __EXIT_PARAM_MISMATCH;

        // eval expression and assign local parameter.
        if (param_count > 0)
            value_array = (struct S_Value**)malloc(sizeof(struct Value*) * param_count);

        current_exp_list = exp->param;
        int current_index = 0;
        while (current_exp_list != 0 && current_exp_list->exp != 0)
        {
            success = S_Eval_Expression(interpreter, current_exp_list->exp);
            if (!success)
                goto __EXIT;
            value_array[current_index] = S_PeekRuntimeStackValue(interpreter, 0);
            current_index++;

            current_exp_list   = current_exp_list->next;
        }
        
        // create local parameter.
        S_ContextPush(interpreter);
        has_pushed_contenxt = true;
        current_param_list = function->u.script.param_list;
        current_index = 0;
        while (current_param_list != 0 && param_count > 0)
        {
            struct S_Local_Variables* variable = S_ContextFindVariable(interpreter,
                current_param_list->symbol->symbol,
                true,    // only local
                true);   // create if not found.
            variable->value = value_array[current_index];
            current_param_list = current_param_list->next;
            current_index++;
        }

        if (is_method)
        {
            struct S_Local_Variables* variable = S_ContextFindVariable(interpreter,
                "this",
                true,    // only local
                true);   // create if not found.
            variable->value = class_instance;
        }

        // eval function body.
        success = S_Eval_Code_Block(interpreter, function->u.script.code_block);
        if (success == true)
        {
            if (S_IsHaveBreak(interpreter) || S_IsHaveContinue(interpreter))
            {
                // unused break or continue in function body. error.
                success = false;
                ERR_Print(ERR_LEVEL_ERROR,
                          "Line %d: unused break or interpreter in function call",
                          exp->header.lineno);
            }
            else if (S_IsHaveReturnValue(interpreter))
            {
                returned_value = S_GetReturnValue(interpreter);
                S_ClearReturnValue(interpreter);
            }
            else
            {
                returned_value = (struct S_Value*)S_CreateValueNil(interpreter);
            }
        }
    }
    else
    {
        DCHECK(false);
        success = false;
    }

__EXIT:
    if (has_pushed_contenxt)
        S_ContextPop(interpreter);
    if (value_array != 0)
        free(value_array);
    while (start_stack_index != S_GetRuntimeStackSize(interpreter))
        S_PopRuntimeStackValue(interpreter);
    if (success)
        S_PushRuntimeStackValue(interpreter, returned_value);
    return success;

__EXIT_PARAM_MISMATCH:
    ERR_Print(ERR_LEVEL_ERROR,
              "Line %d: function parameter mismatch.",
              exp->header.lineno);
    if (has_pushed_contenxt)
        S_ContextPop(interpreter);
    if (value_array != 0)
        free(value_array);
    while (start_stack_index != S_GetRuntimeStackSize(interpreter))
        S_PopRuntimeStackValue(interpreter);

    return false;
}

bool S_Eval_Expression_Array(struct S_Interpreter* interpreter, struct S_Expression_Array* exp)
{
    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);
    bool success = false;

    struct S_Value** value_array = 0;
    unsigned int array_size = 0;

    // count expression.
    S_Expression_List* exp_list = exp->exp_list;
    while (exp_list != 0 && exp_list->exp != 0)
    {
        exp_list = exp_list->next;
        array_size++;
    }

    // construct value_array and calculate array_size.
    if (array_size != 0)
    {
        value_array = (struct S_Value**)malloc(sizeof(struct S_Value*) * array_size);
        exp_list = exp->exp_list;
        int index = 0;
        while (exp_list != 0)
        {
            success = S_Eval_Expression(interpreter, exp_list->exp);
            if (!success)
            {
                goto __EXIT;
            }
            value_array[index] = S_PeekRuntimeStackValue(interpreter, 0);
            index++;
            exp_list = exp_list->next;
        }
        success = true;
    }

__EXIT:
    while (start_stack_index != S_GetRuntimeStackSize(interpreter))
        S_PopRuntimeStackValue(interpreter);

    if (success)
    {
        // create array.
        S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueArray(interpreter, value_array, array_size));
    }

    if (value_array != 0)
    {
        free(value_array);
    }

    return success;
}

bool S_Eval_Expression_Subscript_Range(struct S_Interpreter* interpreter, struct S_Expression_Subscript* exp)
{
    DCHECK(exp->index->header.type == EXPRESSION_TYPE_RANGE);

    bool success = false;
    struct S_Value* returned_value = 0;
    struct S_Value* instance = 0;
    struct S_Expression_Range* range = (struct S_Expression_Range*)exp->index;
    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);

    // get instance
    success = S_Eval_Expression(interpreter, exp->instance);
    if (!success)
        goto __EXIT;

    instance = S_PeekRuntimeStackValue(interpreter, 0);

    // get start index
    unsigned int start_index = 0;
    if (range->exp_start != 0)
    {
        success = S_Eval_Expression(interpreter, range->exp_start);
        if (!success)
            goto __EXIT;

        struct S_Value* start = S_PeekRuntimeStackValue(interpreter, 0);
        if (start->header.type != VALUE_TYPE_INTEGER)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: %s can not be an range index.",
                      range->exp_start->header.lineno,
                      VALUE_NAME[start->header.type]);
            success = false;
            goto __EXIT;
        }

        start_index = (unsigned int)((struct S_Value_Integer*)start)->value;
    }


    // get end index
    unsigned int end_index = 0xffffffff;
    if (range->exp_end != 0)
    {
        success = S_Eval_Expression(interpreter, range->exp_end);
        if (!success)
            goto __EXIT;

        struct S_Value* end = S_PeekRuntimeStackValue(interpreter, 0);
        if (end->header.type != VALUE_TYPE_INTEGER)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: %s can not be an range index.",
                      range->exp_end->header.lineno,
                      VALUE_NAME[end->header.type]);
            success = false;
            goto __EXIT;
        }

        end_index = (unsigned int)((struct S_Value_Integer*)end)->value;
    }

    // invalid range.
    if (start_index > end_index)
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: [%d : %d] is invalid range.",
                  range->header.lineno,
                  start_index,
                  end_index);
        success = false;
        goto __EXIT;
    }


    if (instance->header.type == VALUE_TYPE_STRING)
    {
        struct S_Value_String* value_string = (struct S_Value_String*)instance;
        if (end_index == 0xffffffff)
        {
            end_index = value_string->length;
        }

        if (end_index > value_string->length || start_index > value_string->length)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: string range out of index.",
                      exp->instance->header.lineno);
            success = false;
            goto __EXIT;
        }

        unsigned int length = end_index - start_index;
        if (length == 0)
        {
            returned_value = (struct S_Value*)S_CreateValueString(interpreter, "");
        }
        else if (length == value_string->length)
        {
            returned_value = (struct S_Value*)value_string;
        }
        else
        {
            char* s = (char*)malloc(length + 1);
            memcpy(s, &value_string->string[start_index], length);
            s[length] = 0;
            returned_value = (struct S_Value*)S_CreateValueString(interpreter, s);
            free(s);
        }

        success = true;
    }
    else if (instance->header.type == VALUE_TYPE_ARRAY)
    {
        struct S_Value_Array* value_array = (struct S_Value_Array*)instance;
        if (end_index == 0xffffffff)
        {
            end_index = value_array->array_size;
        }

        if (end_index > value_array->array_size || start_index > value_array->array_size)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: array range out of index.",
                      exp->instance->header.lineno);
            success = false;
            goto __EXIT;
        }

        unsigned int length = end_index - start_index;
        if (length == 0)
        {
            returned_value = (struct S_Value*)S_CreateValueArray(interpreter, 0, 0);
        }
        else
        {
            S_Value** a = (S_Value**)malloc(length * sizeof(S_Value*));
            memcpy(a, &value_array->value_array[start_index], length * sizeof(S_Value*));
            returned_value = (struct S_Value*)S_CreateValueArray(interpreter, a, length);
            free(a);
        }
        success = true;
    }
    else
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s not support range subscription.",
                  exp->instance->header.lineno,
                  VALUE_NAME[instance->header.type]);
        success = false;
    }

__EXIT:
    while(S_GetRuntimeStackSize(interpreter) != start_stack_index)
        S_PopRuntimeStackValue(interpreter);
    if (success && returned_value != 0)
        S_PushRuntimeStackValue(interpreter, returned_value);

    return success;
}

bool S_Eval_Expression_Subscript_Expr(struct S_Interpreter* interpreter, struct S_Expression_Subscript* exp)
{
    bool success = false;
    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);

    struct S_Value* returned_value = 0;
    struct S_Value* index = 0;
    struct S_Value* instance = 0;

    success = S_Eval_Expression(interpreter, exp->instance);
    if (!success)
    {
        goto __EXIT;
    }
    instance = S_PeekRuntimeStackValue(interpreter, 0);

    success = S_Eval_Expression(interpreter, exp->index);
    if (!success)
    {
        goto __EXIT;
    }
    index = S_PeekRuntimeStackValue(interpreter, 0);

    if (index->header.type != VALUE_TYPE_INTEGER)
    {
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: %s can not be an array index.",
                  exp->header.lineno,
                  VALUE_NAME[index->header.type]);
        success = false;
        goto __EXIT;
    }

    struct S_Value_Integer* index_integer = (struct S_Value_Integer*)index;
    if (instance->header.type == VALUE_TYPE_STRING)
    {
        struct S_Value_String* instance_string = (struct S_Value_String*)instance;
        if (index_integer->value < 0 || (unsigned int)index_integer->value >= instance_string->length)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: out of string index.",
                      exp->header.lineno);
            success = false;
            goto __EXIT;
        }

        char c = instance_string->string[index_integer->value];
        returned_value = (struct S_Value*)S_CreateValueChar(interpreter, c);
        success = true;
    }
    else if (instance->header.type == VALUE_TYPE_ARRAY)
    {
        struct S_Value_Array* instance_array = (struct S_Value_Array*)instance;
        if (index_integer->value < 0 || (unsigned int)index_integer->value >= instance_array->array_size)
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: out of array index.",
                      exp->header.lineno);
            success = false;
            goto __EXIT;
        }

        returned_value = instance_array->value_array[index_integer->value];
        success = true;
    }


__EXIT:
    while(start_stack_index != S_GetRuntimeStackSize(interpreter))
        S_PopRuntimeStackValue(interpreter);

    if (success)
    {
        S_PushRuntimeStackValue(interpreter, returned_value);
    }

    return success;
}

bool S_Eval_Expression_Subscript(struct S_Interpreter* interpreter, struct S_Expression_Subscript* exp)
{
    if (exp->index->header.type == EXPRESSION_TYPE_RANGE)
    {
        return S_Eval_Expression_Subscript_Range(interpreter, exp);
    }
    else
    {
        return S_Eval_Expression_Subscript_Expr(interpreter, exp);
    }
}

bool S_Eval_Expression_Char(struct S_Interpreter* interpreter, struct S_Expression_Char* exp)
{
    struct S_Value_Char* c = S_CreateValueChar(interpreter, exp->c);
    DCHECK(c != 0);
    S_PushRuntimeStackValue(interpreter, (struct S_Value*)c);
    return true;
}

bool S_Eval_Expression_Dot(struct S_Interpreter* interpreter, struct S_Expression_Dot* exp)
{
    bool success = false;
    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);
    struct S_Value* returned_value = 0;

    success = S_Eval_Expression(interpreter, exp->instance);
    if (!success)
        goto __EXIT;

    struct S_Value* instance = S_PeekRuntimeStackValue(interpreter, 0);

    struct S_Field_List* field = S_Find_Value_Field(interpreter, instance, exp->field->symbol, false);
    if (field == 0)
    {
        success = false;
        ERR_Print(ERR_LEVEL_ERROR,
            "Line %d: instance %s don't have field %s",
            exp->instance->header.lineno,
            VALUE_NAME[instance->header.type],
            exp->field->symbol);
        goto __EXIT;
    }

    DCHECK(field->value != 0);
    success = true;
    returned_value = field->value;

__EXIT:
    while (start_stack_index != S_GetRuntimeStackSize(interpreter))
        S_PopRuntimeStackValue(interpreter);
    if (success)
    {
        S_PushRuntimeStackValue(interpreter, returned_value);
    }

    return success;
}

bool S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp)
{
    DCHECK(interpreter != 0);
    DCHECK(exp != 0);

    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);
    bool success = 0;
    switch (exp->header.type)
    {
    case EXPRESSION_TYPE_NIL:
        success = S_Eval_Expression_Nil(interpreter, (struct S_Expression_Nil*)exp);
        break;

    case EXPRESSION_TYPE_INTEGER:
        success = S_Eval_Expression_Integer(interpreter, (struct S_Expression_Integer*)exp);
        break;

    case EXPRESSION_TYPE_DOUBLE:
        success = S_Eval_Expression_Double(interpreter, (struct S_Expression_Double*)exp);
        break;

    case EXPRESSION_TYPE_TRUE:
        success = S_Eval_Expression_True(interpreter, (struct S_Expression_True*)exp);
        break;

    case EXPRESSION_TYPE_FALSE:
        success = S_Eval_Expression_False(interpreter, (struct S_Expression_False*)exp);
        break;

    case EXPRESSION_TYPE_STRING:
        success = S_Eval_Expression_String(interpreter, (struct S_Expression_String*)exp);
        break;

    case EXPRESSION_TYPE_SYMBOL:
        success = S_Eval_Expression_Symbol(interpreter, (struct S_Expression_Symbol*)exp);
        break;

    case EXPRESSION_TYPE_NEGATION:
        success = S_Eval_Expression_Negation(interpreter, (struct S_Expression_Negation*)exp);
        break;

    case EXPRESSION_TYPE_OP2:
        success = S_Eval_Expression_Op2(interpreter, (struct S_Expression_Op2*)exp);
        break;

    case EXPRESSION_TYPE_FUNCTION_DEFINE:
        success = S_Eval_Expression_Function_Define(interpreter, (struct S_Expression_Function_Define*)exp);
        break;

    case EXPRESSION_TYPE_FUNCTION_CALL:
        success = S_Eval_Expression_Function_Call(interpreter, (struct S_Expression_Function_Call*)exp);
        break;

    case EXPRESSION_TYPE_ARRAY:
        success = S_Eval_Expression_Array(interpreter, (struct S_Expression_Array*)exp);
        break;

    case EXPRESSION_TYPE_SUBSCRIPT:
        success = S_Eval_Expression_Subscript(interpreter, (struct S_Expression_Subscript*)exp);
        break;

    case EXPRESSION_TYPE_DOT:
        success = S_Eval_Expression_Dot(interpreter, (struct S_Expression_Dot*)exp);
        break;

    case EXPRESSION_TYPE_CHAR:
        success = S_Eval_Expression_Char(interpreter, (struct S_Expression_Char*)exp);
        break;

    default:
        ERR_Print(ERR_LEVEL_ERROR,
                  "Line %d: expression type %d is unsupport",
                  exp->header.lineno,
                  exp->header.type);
        DCHECK(false);
        success = false;
        break;

    }

    if (success)
    {
        DCHECK(S_GetRuntimeStackSize(interpreter) == start_stack_index + 1);
    }
    else
    {
        DCHECK(S_GetRuntimeStackSize(interpreter) == start_stack_index);
    }

    return success;
}

/// End of Evaluate expression ///

/// Evaluate statement ///
bool S_Eval_Statement_Expression(struct S_Interpreter* interpreter, struct S_Statement_Expression* stat)
{
    bool success = S_Eval_Expression(interpreter, stat->exp);
    if (!success)
        return false;

    S_PopRuntimeStackValue(interpreter);
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
    bool success = S_Eval_Expression(interpreter, stat->exp);
    if (!success)
        return false;

    S_SetReturnValue(interpreter, S_PopRuntimeStackValue(interpreter));
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

    return true;
}

bool S_Eval_Statement_While(struct S_Interpreter* interpreter, struct S_Statement_While* stat)
{
    bool success = false;
    struct S_Value* condition_result = 0;
    for (;;)
    {
        // evaluate conditon expression, return if failed.
        success = S_Eval_Expression(interpreter, stat->condition);
        if (!success)
        {
            break;
        }

        condition_result = S_PeekRuntimeStackValue(interpreter, 0);

        if (condition_result->header.type == VALUE_TYPE_TRUE)
        {
            success = S_Eval_Code_Block(interpreter, stat->body);
            if (!success)
            {
                // pop condition_result out.
                S_PopRuntimeStackValue(interpreter);
                break;
            }
            // return or break
            else if (S_IsHaveReturnValue(interpreter))
            {
                S_PopRuntimeStackValue(interpreter);
                break;
            }
            else if (S_IsHaveBreak(interpreter))
            {
                S_PopRuntimeStackValue(interpreter);
                S_ClearBreak(interpreter);
                break;               
            }
            else if (S_IsHaveContinue(interpreter))
            {
                S_ClearContinue(interpreter);
            }

            // pop condition_result out.
            S_PopRuntimeStackValue(interpreter);

            // if success, loop back.
        }
        // condition is false, return.
        else if (condition_result->header.type == VALUE_TYPE_FALSE || condition_result->header.type == VALUE_TYPE_NIL)
        {
            success = true;
            S_PopRuntimeStackValue(interpreter);
            break;
        }
        else
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: %s can not be a condition.",
                      stat->condition->header.lineno,
                      VALUE_NAME[condition_result->header.type]);
            success = false;
            S_PopRuntimeStackValue(interpreter);
            break;
        }
    }

    return success;
}

bool S_Eval_Statement_For(struct S_Interpreter* interpreter, struct S_Statement_For* stat)
{
    bool success = false;
    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);

    if (stat->exp_init != 0)
    {
        success = S_Eval_Expression(interpreter, stat->exp_init);
        if (!success)
            goto __EXIT;
    }

    S_PopRuntimeStackValue(interpreter);    // pop stat->exp_init result out from stack.

    // eval loop
    for (;;)
    {
        struct S_Value* condition_result = 0;
        if (stat->exp_condition != 0)
        {
            success = S_Eval_Expression(interpreter, stat->exp_condition);
            if (!success)
                goto __EXIT;
            condition_result = S_PeekRuntimeStackValue(interpreter, 0);
        }

        // condition is true
        if (condition_result == 0 || condition_result->header.type == VALUE_TYPE_TRUE)
        {
            if (condition_result != 0)
                S_PopRuntimeStackValue(interpreter);    // pop stat->exp_condition result out from stack.

            // eval body
            success = S_Eval_Code_Block(interpreter, stat->body);
            if (!success)
                goto __EXIT;

            // check continue/break/return flag.
            if (S_IsHaveContinue(interpreter))
            {
                S_ClearContinue(interpreter);
            }
            else if (S_IsHaveBreak(interpreter))
            {
                S_ClearBreak(interpreter);
                break;
            }
            else if (S_IsHaveReturnValue(interpreter))
            {
                break;
            }

            // eval next expression
            success = S_Eval_Expression(interpreter, stat->exp_next);
            if (!success)
                goto __EXIT;
            S_PopRuntimeStackValue(interpreter);    // pop stat->exp_next result out from stack

            // loop back
        }

        // condition return false, break the loop
        else if (condition_result->header.type == VALUE_TYPE_FALSE || condition_result->header.type == VALUE_TYPE_NIL)
        {
            success = true;
            break;
        }

        // bad condition
        else
        {
            ERR_Print(ERR_LEVEL_ERROR,
                      "Line %d: %s can not be a condition value.",
                      stat->exp_condition->header.lineno,
                      VALUE_NAME[condition_result->header.type]);
            success = false;
            goto __EXIT;
        }
    }

__EXIT:
    while(S_GetRuntimeStackSize(interpreter) != start_stack_index)
        S_PopRuntimeStackValue(interpreter);

    return success;
}

bool S_Eval_Statement_If(struct S_Interpreter* interpreter, struct S_Statement_If* stat)
{
    bool success = S_Eval_Expression(interpreter, stat->condition);
    if (!success)
    {
        return success;
    }

    struct S_Value* condition_result = S_PeekRuntimeStackValue(interpreter, 0);
    success = false;
    if (condition_result->header.type == VALUE_TYPE_TRUE)
    {
        // Pop condition_result out.
        S_PopRuntimeStackValue(interpreter);
        success = S_Eval_Code_Block(interpreter, stat->body);
    }
    else if (condition_result->header.type == VALUE_TYPE_FALSE || condition_result->header.type == VALUE_TYPE_NIL)
    {
        // Pop condition_result out.
        S_PopRuntimeStackValue(interpreter);
        struct S_Elif_List* elif_list = stat->elif_list;
        while (elif_list != 0)
        {
            success = S_Eval_Expression(interpreter, elif_list->condition);
            if (!success)
            {
                goto __EXIT;
            }

            condition_result = S_PeekRuntimeStackValue(interpreter, 0);
            if (condition_result->header.type == VALUE_TYPE_TRUE)
            {
                // Pop condition_result out.
                S_PopRuntimeStackValue(interpreter);
                success = S_Eval_Code_Block(interpreter, elif_list->body);
                goto __EXIT;
            }
            else
            {
                // Pop condition_result out.
                S_PopRuntimeStackValue(interpreter);
            }

            elif_list = elif_list->next;
        }

        if (stat->else_body != 0)
            success = S_Eval_Code_Block(interpreter, stat->else_body);
        else
            success = true;
    }
    else
    {
        success = false;
        ERR_Print(ERR_LEVEL_ERROR,
                 "Line %d: %s can not be a condition.",
                 stat->condition->header.lineno,
                 VALUE_NAME[condition_result->header.type]);
                // Pop condition_result out.
        S_PopRuntimeStackValue(interpreter);
    }

__EXIT:
    return success;
}

bool S_Eval_Statement_Break(struct S_Interpreter* interpreter, struct S_Statement_Break* stat)
{
    S_SetBreak(interpreter);
    return true;
}

bool S_Eval_Statement_Continue(struct S_Interpreter* interpreter, struct S_Statement_Continue* stat)
{
    S_SetContinue(interpreter);
    return true;
}

bool S_Eval_Statement(struct S_Interpreter* interpreter, struct S_Statement* stat)
{
    DCHECK(interpreter != 0);
    DCHECK(stat != 0);

    unsigned int start_stack_index = S_GetRuntimeStackSize(interpreter);

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

        case STATEMENT_TYPE_FOR:
            eval_success = S_Eval_Statement_For(interpreter, (struct S_Statement_For*)stat);
            break;

        case STATEMENT_TYPE_IF:
            eval_success = S_Eval_Statement_If(interpreter, (struct S_Statement_If*)stat);
            break;

        case STATEMENT_TYPE_BREAK:
            eval_success = S_Eval_Statement_Break(interpreter, (struct S_Statement_Break*)stat);
            break;

        case STATEMENT_TYPE_CONTINUE:
            eval_success = S_Eval_Statement_Continue(interpreter, (struct S_Statement_Continue*)stat);
            break;
            
        default:
            ERR_Print(ERR_LEVEL_ERROR,
                      "Invalid statement type. %d",
                      stat->header.type);
        DCHECK(false);
        break;
    }

    DCHECK(start_stack_index == S_GetRuntimeStackSize(interpreter));
    S_GCIfNeed(interpreter);
    return eval_success;
}

/// End of Evaluate statement


/// Statement list ///
bool S_Eval_Statement_List(struct S_Interpreter* interpreter, struct S_Statement_List* stat_list)
{
    DCHECK(interpreter != 0);
    DCHECK(stat_list != 0);

    // empty list, return true.
    if (stat_list->stat == 0)
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
            break;
        }
        else if (S_IsHaveBreak(interpreter))
        {
            break;
        }
        else if (S_IsHaveContinue(interpreter))
        {
            break;
        }

        current_node = current_node->next;
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

