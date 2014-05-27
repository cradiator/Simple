#pragma once

#include "S_Value.h"
#include "S_Expression.h"

struct S_Interpreter;

bool S_Eval_Statement_List(struct S_Interpreter* interpreter, struct S_Statement_List* stat_list);
