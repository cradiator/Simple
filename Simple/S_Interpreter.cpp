#include "Common.h"
#include "DBG.h"
#include "ERR.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "S_Context.h"
#include "S_Eval.h"

static const unsigned int DEFAULT_GC_WATER_MARK = 0xA00000; // 10MB
static const unsigned int MAX_GC_WATER_MARK = 0x6400000;   // 100MB

EXTERN_C void* yy_scan_bytes(char *base, unsigned int size);
EXTERN_C int yyparse(struct S_Interpreter* interpreter);

void InitRuntimeStackValue(struct S_Interpreter* interpreter);
void MarkRuntimeStack(struct S_Interpreter* interpreter);

void S_GC(struct S_Interpreter* interpreter)
{
    unsigned int storage_size = MM_GetGCStorageSize(interpreter->RunningStorage);
    if (storage_size > DEFAULT_GC_WATER_MARK)
    {
        interpreter->GCWaterMark = storage_size * 2;
    }

    if (interpreter->GCWaterMark > MAX_GC_WATER_MARK)
    {
        interpreter->GCWaterMark = MAX_GC_WATER_MARK;
    }

    // unmark all memory
    MM_UnmarkGCStorage(interpreter->RunningStorage);

    // mark context
    S_MarkContext(interpreter);
    if (S_IsHaveReturnValue(interpreter))
    {
        S_MarkValue(interpreter, interpreter->ReturnValue);
    }

    // mark runtime stack
    MarkRuntimeStack(interpreter);

    // mark cached object
    if (interpreter->NativePrint)
        S_MarkValue(interpreter, (struct S_Value*)interpreter->NativePrint);

    if (interpreter->NativeToString)
        S_MarkValue(interpreter, (struct S_Value*)interpreter->NativeToString);

    if (interpreter->ValueNil)
        S_MarkValue(interpreter, (struct S_Value*)interpreter->ValueNil);

    if (interpreter->ValueTrue)
        S_MarkValue(interpreter, (struct S_Value*)interpreter->ValueTrue);

    if (interpreter->ValueFalse)
        S_MarkValue(interpreter, (struct S_Value*)interpreter->ValueFalse);

    for (int i = 0; i < 256; ++i)
        S_MarkValue(interpreter, (struct S_Value*)interpreter->ValueChar[i]);

    // sweep
    MM_SweepGCMemory(interpreter->RunningStorage);
}

void S_GCIfNeed(struct S_Interpreter* interpreter)
{
    unsigned int storage_size = MM_GetGCStorageSize(interpreter->RunningStorage);
    if (storage_size > interpreter->GCWaterMark)
    {
        printf("gc when 0x%x\n", storage_size);
        S_GC(interpreter);
    }
}


bool S_NativePrint(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count)
{

    for (int i = 0; i < param_count; ++i)
    {
        struct S_Value_String* str = S_ConvertValueToString(interpreter, param_array[i]);
        printf(str->string);
    }

    S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueInteger(interpreter, param_count));
    return true;
}

bool S_NativeToString(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count)
{
    if (param_count != 1)
        return false;

    S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_ConvertValueToString(interpreter, param_array[0]));
    return true;
}

bool S_NativeArray(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count)
{
    if (param_count != 1 || param_array[0]->header.type != VALUE_TYPE_INTEGER)
    {
        ERR_Print(ERR_LEVEL_ERROR,
            "function array only take 1 integer parameter");
        return false;
    }

    struct S_Value_Integer* value_integer = (struct S_Value_Integer*)(param_array[0]);
    S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueArray(interpreter, 0, (unsigned int)value_integer->value));
    return true;
}

bool S_NativeGC(struct S_Interpreter* interpreter, struct S_Value** /*param_array*/, int param_count)
{
    if (param_count != 0)
    {
        ERR_Print(ERR_LEVEL_ERROR,
            "native function gc have no parameter.");
        return false;
    }

    S_GC(interpreter);

    S_PushRuntimeStackValue(interpreter, (struct S_Value*)S_CreateValueNil(interpreter));
    return true;
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
    s->GCWaterMark = DEFAULT_GC_WATER_MARK;

    // Create global context.
    S_ContextPush(s);

    // Create runtime stack.
    InitRuntimeStackValue(s);

    // Push base native function.
    S_AddNativeFunction(s, "print", S_NativePrint);
    S_AddNativeFunction(s, "to_string", S_NativeToString);

    struct S_Local_Variables* variable = 0;
    variable = S_ContextFindVariable(s, "print", true, false);
    DCHECK(variable != 0);
    s->NativePrint = (struct S_Value_Function*)variable->value;

    variable = S_ContextFindVariable(s, "to_string", true, false);
    DCHECK(variable != 0);
    s->NativeToString = (struct S_Value_Function*)variable->value;

    S_Set_Value_Field(s, (struct S_Value*)s->NativePrint, "print", (struct S_Value*)s->NativePrint);
    S_Set_Value_Field(s, (struct S_Value*)s->NativePrint, "to_string", (struct S_Value*)s->NativeToString);

    S_Set_Value_Field(s, (struct S_Value*)s->NativeToString, "print", (struct S_Value*)s->NativePrint);
    S_Set_Value_Field(s, (struct S_Value*)s->NativeToString, "to_string", (struct S_Value*)s->NativeToString);

    // Push common native function.
    S_AddNativeFunction(s, "gc", S_NativeGC);
    S_AddNativeFunction(s, "array", S_NativeArray);

    s->ValueTrue  = S_CreateValueTrue(s);
    s->ValueFalse = S_CreateValueFalse(s);
    s->ValueNil = S_CreateValueNil(s);
    for (int i = 0; i < 256; ++i)
        s->ValueChar[i] = S_CreateValueChar(s, (char)i);

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
    return true;
}

// For rumtime stack
const static int DEFAULT_RUNTIME_STACK_SIZE = 0x1000;
struct RuntimeStack {
    unsigned int total_size;
    unsigned int current_size;
    struct S_Value** stack;
};


void InitRuntimeStackValue(struct S_Interpreter* interpreter)
{
    DCHECK(interpreter->RuntimeStack == 0);

    struct RuntimeStack* rs = 
        (struct RuntimeStack*)MM_Malloc(interpreter->RunningStorage, sizeof(struct RuntimeStack));
    rs->total_size   = DEFAULT_RUNTIME_STACK_SIZE;
    rs->current_size = 0;
    rs->stack = (struct S_Value**)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value**) * DEFAULT_RUNTIME_STACK_SIZE);

    interpreter->RuntimeStack = rs;
}

void MarkRuntimeStack(struct S_Interpreter* interpreter)
{
    DCHECK(interpreter->RuntimeStack != 0);
    struct RuntimeStack* stack = (struct RuntimeStack*)interpreter->RuntimeStack;

    MM_MarkGCMemory(interpreter->RunningStorage, stack);
    if (stack->stack != 0)
    {
        MM_MarkGCMemory(interpreter->RunningStorage, stack->stack);
    }

    if (stack->current_size == 0)
    {
        return;
    }

    for(unsigned int i = 0; i < stack->current_size; ++i)
    {
        S_MarkValue(interpreter, stack->stack[i]);
    }
}

void GrowRuntimeStack(struct S_Interpreter* interpreter)
{
    struct RuntimeStack* old_stack = (struct RuntimeStack*)interpreter->RuntimeStack;
    
    unsigned int new_size = old_stack->total_size + DEFAULT_RUNTIME_STACK_SIZE;
    struct S_Value** new_stack_array = 
        (struct S_Value**)MM_Malloc(interpreter->RunningStorage,
                                    sizeof(struct S_Value*) * new_size);

    memcpy(new_stack_array, old_stack->stack, old_stack->current_size * sizeof(struct S_Value*));
    old_stack->stack = new_stack_array;
    old_stack->total_size = new_size;
}

void GrowRuntimeStackIfNeed(struct S_Interpreter* interpreter)
{
    struct RuntimeStack* stack = (struct RuntimeStack*)interpreter->RuntimeStack;
    if (stack->current_size >= stack->total_size)
        GrowRuntimeStack(interpreter);
}

void S_PushRuntimeStackValue(struct S_Interpreter* interpreter, struct S_Value* value)
{
    DCHECK(value != 0);

    GrowRuntimeStackIfNeed(interpreter);
    struct RuntimeStack* stack = (struct RuntimeStack*)interpreter->RuntimeStack;
    DCHECK(stack->current_size < stack->total_size);

    stack->stack[stack->current_size++] = value;
}

struct S_Value* S_PopRuntimeStackValue(struct S_Interpreter* interpreter)
{
    struct RuntimeStack* stack = (struct RuntimeStack*)interpreter->RuntimeStack;
    DCHECK(stack->current_size > 0);

    struct S_Value* returned_value = stack->stack[stack->current_size - 1];
    stack->current_size--;
    return returned_value;
}

unsigned int S_GetRuntimeStackSize(struct S_Interpreter* interpreter)
{
    struct RuntimeStack* stack = (struct RuntimeStack*)interpreter->RuntimeStack;
    return stack->current_size;
}

struct S_Value* S_PeekRuntimeStackValue(struct S_Interpreter* interpreter, unsigned int index)
{
    struct RuntimeStack* stack = (struct RuntimeStack*)interpreter->RuntimeStack;
    if (index >= stack->current_size)
        return 0;

    unsigned int real_index = stack->current_size - 1 - index;
    return stack->stack[real_index];
}
