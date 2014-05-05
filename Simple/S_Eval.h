#pragma once

#include "S_Value.h"
#include "S_Expression.h"

struct S_Interpreter;

struct S_Value* S_Eval_Expression(struct S_Interpreter* interpreter, struct S_Expression* exp);

