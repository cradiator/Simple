#pragma once

struct S_Interpreter;
struct S_Expression_Function_Define;

enum S_Value_Type {
    VALUE_TYPE_MIN,
    VALUE_TYPE_NIL = VALUE_TYPE_MIN,
    VALUE_TYPE_TRUE,
    VALUE_TYPE_FALSE,
    VALUE_TYPE_INTEGER,
    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_STRING,
    VALUE_TYPE_CHAR,
    VALUE_TYPE_SYMBOL,
    VALUE_TYPE_FUNCTION,
    VALUE_TYPE_ARRAY,
    VALUE_TYPE_MAX,
};

__declspec(selectany) 
const char* VALUE_NAME[] = {
    "nil",
    "true",
    "false",
    "integer",
    "double",
    "string",
    "char",
    "symbol",
    "function",
    "array",
};

struct S_Field_List {
    struct S_Field_List* next;
    char* name;
    struct S_Value* value;
};

struct S_Value_Header {
    enum S_Value_Type type;
    struct S_Field_List* field_list;
};

struct S_Value {
    struct S_Value_Header header;
};

struct S_Value_Nil {
    struct S_Value_Header header;
};

struct S_Value_True {
    struct S_Value_Header header;
};

struct S_Value_False {
    struct S_Value_Header header;
};

struct S_Value_Integer {
    struct S_Value_Header header;
    int value;
};

struct S_Value_Double {
    struct S_Value_Header header;
    double value;
};

struct S_Value_String {
    struct S_Value_Header header;
    char* string;
    unsigned int length;
};

struct S_Value_Char {
    struct S_Value_Header header;
    char c;
};

struct S_Value_Symbol {
    struct S_Value_Header header;
    char* symbol;
};

struct S_Value_Array {
    struct S_Value_Header header;
    struct S_Value** value_array;
    unsigned int array_size;
};

enum {
    SCRIPT_FUNCTION,
    NATIVE_FUNCTION,
};

#ifdef __cplusplus
typedef bool (*S_NativeFunctionProto)(struct S_Interpreter* interpreter, struct S_Value** param_array, int param_count);

struct S_Value_Function {
    struct S_Value_Header header;
    int type;
    union {
        S_NativeFunctionProto native;
        struct {
            struct S_Parameter_List* param_list;
            struct S_Code_Block* code_block;
        } script;
    } u;
};

struct S_Field_List* S_Find_Value_Field(struct S_Interpreter* interpreter, struct S_Value* value, const char* field_name, bool create_if_not_exist);
void S_Set_Value_Field(struct S_Interpreter* interpreter, struct S_Value* value, const char* field_name, struct S_Value* field_value);

#else
struct S_Value_Funtion;
#endif

// S_CreateValueXxx routine's returned value could be gabbage collected by default.
// If you want to pin it in memeory, push it onto runtime stack or assign it in context variable. 
struct S_Value_Nil* S_CreateValueNil(struct S_Interpreter* interpreter);

struct S_Value_True* S_CreateValueTrue(struct S_Interpreter* interpreter);

struct S_Value_False* S_CreateValueFalse(struct S_Interpreter* interpreter);

struct S_Value_Integer* S_CreateValueInteger(struct S_Interpreter* interpreter, int value);

struct S_Value_Double* S_CreateValueDouble(struct S_Interpreter* interpreter, double value);

struct S_Value_String* S_CreateValueString(struct S_Interpreter* interpreter, const char* string);

struct S_Value_Char* S_CreateValueChar(struct S_Interpreter* interpreter, char c);

struct S_Value_Symbol* S_CreateValueSymbol(struct S_Interpreter* interpreter, const char* symbol);

struct S_Value_Function* S_CreateValueFunction(struct S_Interpreter* interpreter, struct S_Parameter_List* param_list, struct S_Code_Block* code_block);

#ifdef __cplusplus
struct S_Value_Function* S_CreateValueNativeFunction(struct S_Interpreter* interpreter, S_NativeFunctionProto function);
#endif

// if value_array is NULL, would create an array all of whose elements is S_Value_Nil.
struct S_Value_Array* S_CreateValueArray(struct S_Interpreter* interpreter, struct S_Value** value_array, unsigned int array_size);

// For mark-sweep process.
void S_MarkValue(struct S_Interpreter* interpreter, struct S_Value* value);

struct S_Value_String* S_ConvertValueToString(struct S_Interpreter* interpreter, struct S_Value* value);
