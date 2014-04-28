#include "Common.h"
#include "MM.h"
#include "DBG.h"
#include "S_Expression.h"

struct S_Expression_Null* CreateExpressionNull(S_Interpreter* interpreter)
{
    struct S_Expression_Null* e = MM_AllocateStorage(interpreter->ParsingStorage, 
                                                    sizeof(struct S_Expression_Null));
    DCHECK(e != 0);

    e->header->type = EXPRESSION_TYPE_NULL;
    return e;
}

struct S_Expression_Integer* CreateExpressionInteger(S_Interpreter* interpreter, int value)
{
    struct S_Expression_Integer* e = 
        (struct S_Expression_Integer*)MM_AllocateStorage(interpreter->ParsingStorage,
                                                         sizeof(struct S_Expression_Integer));
}