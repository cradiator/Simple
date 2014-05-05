#pragma once

struct S_Interpreter;
struct S_Expression_Function_Define;

enum {
    VALUE_TYPE_NIL,
    VALUE_TYPE_TRUE,
    VALUE_TYPE_FALSE,
    VALUE_TYPE_INTEGER,
    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_STRING,
    VALUE_TYPE_SYMBOL,
    VALUE_TYPE_FUNCTION,
};

struct S_Value_Header {
    int type;
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
};

struct S_Value_Symbol {
    struct S_Value_Header header;
    char* symbol;
};

struct S_Value_Function {
    struct S_Value_Header header;
    struct S_Expression_Function_Define* function;
};

struct S_Value_Nil* S_CreateValueNil(struct S_Interpreter* interpreter);

struct S_Value_True* S_CreateValueTrue(struct S_Interpreter* interpreter);

struct S_Value_False* S_CreateValueFalse(struct S_Interpreter* interpreter);

struct S_Value_Integer* S_CreateValueInteger(struct S_Interpreter* interpreter, int value);

struct S_Value_Double* S_CreateValueDouble(struct S_Interpreter* interpreter, double value);

// This routine would take ownership of string.
struct S_Value_String* S_CreateValueString(struct S_Interpreter* interpreter, char* string);

// This routine would take ownership of string.
struct S_Value_Symbol* S_CreateValueSymbol(struct S_Interpreter* interpreter, char* symbol);

struct S_Value_Function* S_CreateValueFunction(struct S_Interpreter* interpreter, struct S_Expression_Function_Define* function);
