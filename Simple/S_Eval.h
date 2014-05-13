#pragma once

#include "S_Value.h"
#include "S_Expression.h"

struct S_Interpreter;

// S_Value returned by S_Eval_Expression is not marked collectable.
struct S_Value* S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp);


bool S_Eval_Statement_List(struct S_Interpreter* interpreter, struct S_Statement_List* stat_list);