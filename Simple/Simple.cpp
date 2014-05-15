#include "Common.h"
#include "DBG.h"
#include "MM.h"
#include "ERR.h"
#include "S_Interpreter.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        ERR_Print(ERR_LEVEL_ERROR, "%s filename\n", argv[0]);
        exit(-1);
    }

	struct S_Interpreter* interpreter = S_NewInterpreter();
    int status = S_DoCompileFile(interpreter, argv[1]);
    if (status != ERR_CODE_SUCCESS)
    {
        return -1;
    }

    status = S_Run(interpreter);
    if (status != ERR_CODE_SUCCESS)
    {
        return -1;
    }

    // MM_DumpGCMemory(interpreter->RunningStorage);

	return 0;
}

