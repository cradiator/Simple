#include "Common.h"
#include "DBG.h"
#include "ERR.h"
#include "S_Interpreter.h"

extern "C" void* yy_scan_buffer(char *base, unsigned int size);

int CompileInternal(struct S_Interpreter* interpreter)
{
    DCHECK(interpreter->Flag & INTERPRETER_FLAG_HAVE_SOURCE);
    yy_scan_buffer(interpreter->SourceFileContent, interpreter->SourceSize);

    return 0;
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

    FILE* fp = fopen(filename, "r");
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
    DCHECK(interpreter->SourceFileContent);

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

