#include "Common.h"
#include "MM.h"
#include "DBG.h"
#include "S_Expression.h"
#include "S_Interpreter.h"

struct S_Expression_Nil* CreateExpressionNil(S_Interpreter* interpreter)
{
    struct S_Expression_Nil* e = 
        (struct S_Expression_Nil*)MM_AllocateStorage(interpreter->ParsingStorage, 
                                                      sizeof(struct S_Expression_Nil));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_NIL;
    return e;
}

struct S_Expression_Integer* CreateExpressionInteger(S_Interpreter* interpreter, int value)
{
    struct S_Expression_Integer* e = 
        (struct S_Expression_Integer*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                         sizeof(struct S_Expression_Integer));
    DCHECK(e != 0);
    e->header.type = EXPRESSION_TYPE_INTEGER;
    e->value = value;
    return e;
}

struct S_Expression_Double* CreateExpressionDouble(S_Interpreter* interpreter, double value)
{
    struct S_Expression_Double* e =
        (struct S_Expression_Double*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                        sizeof(struct S_Expression_Double));
    DCHECK(e != 0);

    e->header.type = EXPRESSION_TYPE_DOUBLE;
    e->value = value;
    return e;
}

struct S_Expression_String* CreateExpressionString(struct S_Interpreter* interpreter, char* string)
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

struct S_Expression_Symbol* CreateExpressionSymbol(struct S_Interpreter* interpreter, char* symbol)
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

