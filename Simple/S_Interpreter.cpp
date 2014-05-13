#include "Common.h"
#include "DBG.h"
#include "ERR.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "S_Context.h"
#include "S_Eval.h"

EXTERN_C void* yy_scan_bytes(char *base, unsigned int size);
EXTERN_C int yyparse(struct S_Interpreter* interpreter);

struct S_Value* S_NativePrint(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count)
{
    if (param_count != 1)
        return 0;

    struct S_Value* param = param_array[0];
    switch (param->header.type)
    {
    case VALUE_TYPE_STRING:
        printf("%s\n", ((struct S_Value_String*)param)->string);
        break;

    case VALUE_TYPE_INTEGER:
        printf("%d\n", ((struct S_Value_Integer*)param)->value);
        break;

    case VALUE_TYPE_DOUBLE:
        printf("%lf\n", ((struct S_Value_Double*)param)->value);
        break;

    case VALUE_TYPE_TRUE:
        printf("true\n");
        break;

    case VALUE_TYPE_FALSE:
        printf("false\n");
        break;

    case VALUE_TYPE_NIL:
        printf("nil\n");
        break;

    default:
        printf("type %s\n", VALUE_NAME[param->header.type]);
        break;
    }

    return (struct S_Value*)S_CreateValueNil(interpreter);
}

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

    // Create global context.
    S_ContextPush(s);

    // Push native function.
    S_AddNativeFunction(s, "Print", S_NativePrint);
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

int S_Run(struct S_Interpreter* interpreter)
{
    if ((interpreter->Flag & INTERPRETER_FLAG_HAVE_SOURCE) == 0)
    {
        return ERR_CODE_NO_SCRIPT;
    }

    // Clear any pending return value.
    if (S_IsHaveReturnValue(interpreter))
        S_ClearReturnValue(interpreter);

    // Run every statement
    struct S_Statement_List* stat_list = interpreter->StatementList;
    DCHECK(stat_list != 0);
    if (stat_list->stat == 0)
    {
        return ERR_CODE_SUCCESS;
    }

    bool success = S_Eval_Statement_List(interpreter, stat_list);
    if (success == false)
    {
        return ERR_CODE_RUNTIME_ERROR;
    }

    return ERR_CODE_SUCCESS;
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

    if ((interpreter->Flag & INTERPRETER_FLAG_HAVE_RETURN) == 0)
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

bool S_AddNativeFunction(struct S_Interpreter* interpreter, const char* name, S_NativeFunctionProto native_function)
{
    struct S_Local_Variables* variable = S_ContextFindVariable(interpreter, name, true, true);
    DCHECK(variable != 0);

    variable->value = (struct S_Value*)S_CreateValueNativeFunction(interpreter, native_function);
    S_MarkValueCollectable(interpreter, variable->value);
    return true;
}

