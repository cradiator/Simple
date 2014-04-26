#include "Common.h"
#include "DBG.h"
#include "S_Interpreter.h"

struct S_Interpreter* S_NewInterpreter()
{
	struct S_Interpreter* s = (struct S_Interpreter*)malloc(sizeof(struct S_Interpreter));
	memset(s, 0, sizeof(*s));

	s->ParsingStorage = MM_NewStorage();
	DCHECK(s->ParsingStorage != 0);

	s->RunningStorage = MM_NewGCStorage();
	DCHECK(s->RunningStorage != 0);

	return s;
}

