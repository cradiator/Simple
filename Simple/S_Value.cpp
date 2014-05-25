#include "Common.h"
#include "MM.h"
#include "DBG.h"
#include "S_Interpreter.h"
#include "S_Value.h"
#include "ERR.h"
#include <vector>
#include <string>

void InitializeValueHeader(struct S_Interpreter* interpreter, struct S_Value* value, enum S_Value_Type type)
{
    DCHECK(value != 0);
    DCHECK(type >= VALUE_TYPE_MIN && type < VALUE_TYPE_MAX);

    value->header.type = type;
    value->header.field_list = 0;


    if (interpreter->NativePrint != 0)
        S_Set_Value_Field(interpreter, value, "print", (struct S_Value*)interpreter->NativePrint);

    if (interpreter->NativeToString != 0)
        S_Set_Value_Field(interpreter, value, "to_string", (struct S_Value*)interpreter->NativeToString);
}

struct S_Value_Nil* S_CreateValueNil(struct S_Interpreter* interpreter)
{
    if (interpreter->ValueNil != 0)
        return interpreter->ValueNil;

    struct S_Value_Nil* v = 
        (struct S_Value_Nil*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Nil));

    DCHECK(v != 0);
    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_NIL);

    interpreter->ValueNil = v;
    return v;
}


struct S_Value_True* S_CreateValueTrue(struct S_Interpreter* interpreter)
{
    if (interpreter->ValueTrue != 0)
        return interpreter->ValueTrue;

    struct S_Value_True* v = 
        (struct S_Value_True*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_True));

    DCHECK(v != 0);
    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_TRUE);

    interpreter->ValueTrue = v;
    return v;
}

struct S_Value_False* S_CreateValueFalse(struct S_Interpreter* interpreter)
{
    if (interpreter->ValueFalse != 0)
        return interpreter->ValueFalse;

    struct S_Value_False* v = 
        (struct S_Value_False*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_False));

    DCHECK(v != 0);
    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_FALSE);

    interpreter->ValueFalse = v;
    return v;
}

struct S_Value_Integer* S_CreateValueInteger(struct S_Interpreter* interpreter, int value)
{
    struct S_Value_Integer* v = 
        (struct S_Value_Integer*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Integer));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_INTEGER);
    v->value = value;
    return v;
}

struct S_Value_Double* S_CreateValueDouble(struct S_Interpreter* interpreter, double value)
{
    struct S_Value_Double* v = 
        (struct S_Value_Double*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Double));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_DOUBLE);
    v->value = value;
    return v;
}

bool S_NativeMethodStringSize(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count)
{
    if (param_count != 1 || param_array[0]->header.type != VALUE_TYPE_STRING)
    {
        ERR_Print(ERR_LEVEL_ERROR,
            "string.size() should be called as an string method and with no parameter.");
        return false;
    }

    struct S_Value_String* str = (struct S_Value_String*)(param_array[0]);
    struct S_Value_Integer* s = S_CreateValueInteger(interpreter, (int)str->length);

    S_PushRuntimeStackValue(interpreter, (struct S_Value*)s);
    return true;
}

struct S_Value_String* S_CreateValueString(struct S_Interpreter* interpreter, const char* string)
{
    DCHECK(string != 0);

    struct S_Value_String* v =
        (struct S_Value_String*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_String));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_STRING);
    v->string = MM_CopyString(interpreter->RunningStorage, string);
    v->length = strlen(v->string);

    S_Set_Value_Field(interpreter, (struct S_Value*)v, "size", (struct S_Value*)S_CreateValueNativeFunction(interpreter, S_NativeMethodStringSize));
    return v;
}

struct S_Value_Char* S_CreateValueChar(struct S_Interpreter* interpreter, char c)
{
    if (interpreter->ValueChar[c] != 0)
        return interpreter->ValueChar[c];

    struct S_Value_Char* v =
        (struct S_Value_Char*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Char));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_CHAR);
    v->c = c;
    return v;
}

struct S_Value_Symbol* S_CreateValueSymbol(struct S_Interpreter* interpreter, const char* symbol)
{
    DCHECK(symbol != 0);
    struct S_Value_Symbol* v =
        (struct S_Value_Symbol*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Symbol));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_SYMBOL);
    v->symbol = MM_CopyString(interpreter->RunningStorage, symbol);
    return v;
}

struct S_Value_Function* S_CreateValueFunction(struct S_Interpreter* interpreter, struct S_Parameter_List* param_list, struct S_Code_Block* code_block)
{
    struct S_Value_Function* v =
        (struct S_Value_Function*)MM_Malloc(interpreter->RunningStorage, sizeof(S_Value_Function));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_FUNCTION);
    v->type = SCRIPT_FUNCTION;
    v->u.script.param_list = param_list;
    v->u.script.code_block = code_block;

    return v;
}

struct S_Value_Function* S_CreateValueNativeFunction(struct S_Interpreter* interpreter, S_NativeFunctionProto function)
{
    DCHECK(function != 0);
    struct S_Value_Function* v =
        (struct S_Value_Function*)MM_Malloc(interpreter->RunningStorage, sizeof(S_Value_Function));
    DCHECK(v != 0);

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_FUNCTION);
    v->type = NATIVE_FUNCTION;
    v->u.native = function;

    return v;
}

bool S_NativeMethodArraySize(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count)
{
    if (param_count != 1 || param_array[0]->header.type != VALUE_TYPE_ARRAY)
    {
        ERR_Print(ERR_LEVEL_ERROR,
            "array.size() should be called as an array method and with no parameter.");
        return false;
    }

    struct S_Value_Array* array = (struct S_Value_Array*)(param_array[0]);
    struct S_Value_Integer* s = S_CreateValueInteger(interpreter, (int)array->array_size);

    S_PushRuntimeStackValue(interpreter, (struct S_Value*)s);
    return true;
}

struct S_Value_Array* S_CreateValueArray(struct S_Interpreter* interpreter, struct S_Value** value_array, unsigned int array_size)
{
    DCHECK(value_array != 0 || array_size != 0);

    struct S_Value_Array* v = 
        (struct S_Value_Array*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value_Array));
    DCHECK(v != 0);

    struct S_Value** array = (struct S_Value**)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Value*) * array_size);
    DCHECK(array != 0);

    if (value_array != 0)
    {
        memcpy(array, value_array, sizeof(struct S_Value*) * array_size);
    }
    else
    {
        for (unsigned int i = 0; i < array_size; ++i)
            array[i] = (struct S_Value*)S_CreateValueNil(interpreter);
    }

    InitializeValueHeader(interpreter, (struct S_Value*)v, VALUE_TYPE_ARRAY);
    v->value_array = array;
    v->array_size = array_size;

    S_Set_Value_Field(interpreter, (struct S_Value*)v, "size", (struct S_Value*)S_CreateValueNativeFunction(interpreter, S_NativeMethodArraySize));
    return v;
}

void ConvertValueToStringHelper(struct S_Value* value, std::string& result)
{
    int value_type = value->header.type;
    if (value_type == VALUE_TYPE_STRING)
    {
        result += ((struct S_Value_String*)value)->string;
    }
    else if (value_type == VALUE_TYPE_INTEGER)
    {
        char buf[32] = { 0 };
        sprintf_s(buf, "%d", ((struct S_Value_Integer*)value)->value);
        result += buf;
    }
    else if (value_type == VALUE_TYPE_DOUBLE)
    {
        char buf[32];
        sprintf_s(buf, "%lf", ((struct S_Value_Double*)value)->value);
        result += buf;
    }
    else if (value_type == VALUE_TYPE_ARRAY)
    {
        struct S_Value_Array* array = (struct S_Value_Array*)value;
        result += '[';
        for (unsigned int i = 0; i < array->array_size; ++i)
        {
            ConvertValueToStringHelper(array->value_array[i], result);
            if (i != array->array_size - 1)
                result += ", ";
        }
        result += ']';
    }
    else if (value_type == VALUE_TYPE_CHAR)
    {
        struct S_Value_Char* c = (struct S_Value_Char*)value;
        result += c->c;
    }
    else
    {
        result += VALUE_NAME[value_type];
    }
}

struct S_Value_String* S_ConvertValueToString(struct S_Interpreter* interpreter, struct S_Value* value)
{
    DCHECK(value != 0);
    DCHECK(interpreter != 0);

    std::string tmp_string;
    ConvertValueToStringHelper(value, tmp_string);

    return S_CreateValueString(interpreter, tmp_string.c_str());
}

void S_MarkValue(struct S_Interpreter* interpreter, struct S_Value* value)
{
    DCHECK(value != 0);
    if (MM_IsGCMemoryMarked(interpreter->RunningStorage, value))
        return;

    MM_MarkGCMemory(interpreter->RunningStorage, value);

    if (value->header.type == VALUE_TYPE_STRING)
    {
        struct S_Value_String* s = (struct S_Value_String*)value;
        MM_MarkGCMemory(interpreter->RunningStorage, s->string);
    }
    else if (value->header.type == VALUE_TYPE_SYMBOL)
    {
        struct S_Value_Symbol* s = (struct S_Value_Symbol*)value;
        MM_MarkGCMemory(interpreter->RunningStorage, s->symbol);
    }
    else if (value->header.type == VALUE_TYPE_ARRAY)
    {
        struct S_Value_Array* s = (struct S_Value_Array*)value;
        if (s->value_array != 0)
            MM_MarkGCMemory(interpreter->RunningStorage, s->value_array);
        for (unsigned int i = 0; i < s->array_size; ++i) 
        {
            S_MarkValue(interpreter, (s->value_array)[i]);
        }
    }

    // mark field list
    struct S_Field_List* field = value->header.field_list;
    while (field != 0)
    {
        MM_MarkGCMemory(interpreter->RunningStorage, field);
        MM_MarkGCMemory(interpreter->RunningStorage, field->name);
        S_MarkValue(interpreter, field->value);
        field = field->next;
    }
}

struct S_Field_List* S_Find_Value_Field(struct S_Interpreter* interpreter, struct S_Value* value, const char* field_name, bool create_if_not_exist)
{
    DCHECK(interpreter != 0);
    DCHECK(value != 0);
    
    if (field_name == 0 || field_name[0] == 0)
    {
        DCHECK(!create_if_not_exist);
        return 0;
    }

    struct S_Field_List* current_node = value->header.field_list;
    bool found = false;
    while (current_node != 0)
    {
        if (strcmp(current_node->name, field_name) == 0)
        {
            found = true;
            break;
        }

        current_node = current_node->next;
    }

    if (found)
        return current_node;

    if (create_if_not_exist)
    {
        current_node = (struct S_Field_List*)MM_Malloc(interpreter->RunningStorage, sizeof(struct S_Field_List));
        DCHECK(current_node != 0);
        current_node->next = value->header.field_list;
        current_node->name = MM_CopyString(interpreter->RunningStorage, field_name);
        current_node->value = 0;
        value->header.field_list = current_node;
    }
    else
    {
        current_node = 0;
    }

    return current_node;
}

void S_Set_Value_Field(struct S_Interpreter* interpreter, struct S_Value* value, const char* field_name, struct S_Value* field_value)
{
    DCHECK(field_value != 0);

    struct S_Field_List* field = S_Find_Value_Field(interpreter, value, field_name, true);
    DCHECK(field != 0);

    field->value = field_value;
}

