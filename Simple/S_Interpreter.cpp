#include "Common.h"
#include "DBG.h"
#include "ERR.h"
#include "S_Interpreter.h"
#include "S_Value.h"

EXTERN_C void* yy_scan_bytes(char *base, unsigned int size);
EXTERN_C int yyparse(struct S_Interpreter* interpreter);

int CompileInternal(struct S_Interpreter* interpreter)
{
    DCHECK((interpreter->Flag & INTERPRETER_FLAG_HAVE_SOURCE) != 0);
	yy_scan_bytes(interpreter->SourceFileContent, interpreter->SourceSize);

    if (yyparse(interpreter) != 0)
    {
        return ERR_CODE_COMPILE_SOURCE_ERROR;
    }

    return ERR_CODE_SUCCESS;
}

struct S_Interpreter* S_NewInterpreter()
{
	struct S_Interpreter* s = (struct S_Interpreter*)malloc(sizeof(struct S_Interpreter));
	memset(s, 0, sizeof(*s));

	s->ParsingStorage = MM_NewStorage(0);
	DCHECK(s->ParsingStorage != 0);

	s->RunningStorage = MM_NewGCStorage();
	DCHECK(s->RunningStorage != 0);

	s->LineNo = 1;
	return s;
}

int S_DoCompileFile(struct S_Interpreter* interpreter, const char* filename)
{
    // read the file content out.
    if (filename == 0)
    {
        return ERR_CODE_FILE_NOT_FOUND;
    }

	FILE* fp = 0;
	fopen_s(&fp, filename, "rb");
    if (fp == 0)
    {
        return ERR_CODE_OPEN_FILE_FAILED;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    if (filesize <= 0)
    {
        fclose(fp);
        return ERR_CODE_INVALID_FILE_SIZE;
    }

    char* file_content = (char*)malloc(filesize + 1);
    DCHECK(file_content != 0);
    memset(file_content, 0, filesize + 1);

    fseek(fp, 0, SEEK_SET);
    fread(file_content, 1, filesize, fp);
    fclose(fp);

    // Ensure no pending parsing.
    if (interpreter->Flag & INTERPRETER_FLAG_HAVE_SOURCE)
    {
        if (interpreter->SourceFileContent)
            free(interpreter->SourceFileContent);
        interpreter->SourceFileContent = 0;
    }
    DCHECK(interpreter->SourceFileContent == 0);

    // Init everything.
    interpreter->Flag |= INTERPRETER_FLAG_HAVE_SOURCE;
    interpreter->SourceFileContent = file_content;
    interpreter->SourceSize = filesize;
    interpreter->LineNo = 1;

    // compiling it.
    return CompileInternal(interpreter);
}

void S_IncSrcLineNo(struct S_Interpreter* interpreter)
{
	interpreter->LineNo++;
}

int S_GetSrcLineNo(struct S_Interpreter* interpreter)
{
	return interpreter->LineNo;
}

void yyerror(struct S_Interpreter* interpreter, const char* message, ...)
{
	static char buf[128];
	va_list ap;
	va_start(ap, message);
	vsprintf_s(buf, message, ap);
    ERR_Print(ERR_LEVEL_ERROR, "Line %d: %s\n", S_GetSrcLineNo(interpreter), buf);
	va_end(ap);
}

void S_SetReturnValue(struct S_Interpreter* interpreter, S_Value* value)
{
    DCHECK((interpreter->Flag & INTERPRETER_FLAG_HAVE_RETURN) == 0);
    DCHECK(value != 0);

    interpreter->ReturnValue = value;
    interpreter->Flag |= INTERPRETER_FLAG_HAVE_RETURN;
}

struct S_Value* S_GetReturnValue(struct S_Interpreter* interpreter)
{
    DCHECK((interpreter->Flag & INTERPRETER_FLAG_HAVE_RETURN) != 0);

    if ((interpreter->Flag & INTERPRETER_FLAG_HAVE_RETURN) != 0)
        return 0;

    return interpreter->ReturnValue;
}

void S_ClearReturnValue(struct S_Interpreter* interpreter)
{
    DCHECK((interpreter->Flag & INTERPRETER_FLAG_HAVE_RETURN) != 0);
    interpreter->ReturnValue = 0;
    interpreter->Flag &= ~INTERPRETER_FLAG_HAVE_RETURN;
}

bool S_IsHaveReturnValue(struct S_Interpreter* interpreter)
{
    if ((interpreter->Flag & INTERPRETER_FLAG_HAVE_RETURN) != 0)
    {
        DCHECK(interpreter->ReturnValue != 0);
        return true;
    }

    return false;
}
