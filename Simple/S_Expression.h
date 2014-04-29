#pragma once

//// Declearation  ////
struct S_Expression_List;
struct S_Parameter_List;
struct S_Expression_Header;

//// End od Declearation ////

//// Expression /////
enum {
    EXPRESSION_TYPE_NIL = 0,
    EXPRESSION_TYPE_INTEGER,
    EXPRESSION_TYPE_DOUBLE,
    EXPRESSION_TYPE_STRING,
    EXPRESSION_TYPE_SYMBOL,
    EXPRESSION_TYPE_NEGATION,
    EXPRESSION_TYPE_FUNCTION_CALL,
    EXPRESSION_TYPE_OP2,
};

enum {
    OP2_MIN = 0,
    OP2_ASSIGN = OP2_MIN,
    OP2_G,
    OP2_GE,
    OP2_E,
    OP2_L,
    OP2_LE,
    OP2_NE,
    OP2_ADD,
    OP2_SUB,
    OP2_MUL,
    OP2_DIV,
    OP2_FACTOR,
    OP2_MAX,
};

struct S_Expression_Header {
    int type;
};

struct S_Expresstion {
    struct S_Expression_Header header;
};

struct S_Expression_Nil {
    struct S_Expression_Header header;
};

struct S_Expression_Integer {
    struct S_Expression_Header header;
    int value;
};

struct S_Expression_Double {
    struct S_Expression_Header header;
    double value;
};

struct S_Expression_String {
    struct S_Expression_Header header;
    int   length;
    char* string;
};

struct S_Expression_Symbol {
    struct S_Expression_Header header;
    char* symbol;
};

struct S_Expression_Negation {
    struct S_Expression_Header header;
    struct S_Expression *exp;
};

struct S_Expression_Function_Call {
    struct S_Expression_Header header;
    struct S_Expression *fn;
    struct S_Expression_List *param;
};

struct S_Expression_Op2 {
    struct S_Expression_Header header;
    struct S_Expression *exp1;
    struct S_Expression *exp2;
    int op;
};


struct S_Expression_Nil* S_CreateExpressionNil(struct S_Interpreter* interpreter);

struct S_Expression_Integer* S_CreateExpressionInteger(struct S_Interpreter* interpreter, int value);

struct S_Expression_Double* S_CreateExpressionDouble(struct S_Interpreter* interpreter, double value);

// This function would take ownership of string.
struct S_Expression_String* S_CreateExpressionString(struct S_Interpreter* interpreter, char* string);

// This function would take ownership of symbol.
struct S_Expression_Symbol* S_CreateExpressionSymbol(struct S_Interpreter* interpreter, char* symbol);

struct S_Expression_Negation* S_CreateExpressionNegation(struct S_Interprete* interpreter, struct S_Expression* exp);

struct S_Expression_Function_Call* S_CreateExpressionFunctionCall(struct S_Interpreter* interpreter,
                                                                  struct S_Expression *fn,
                                                                  struct S_Expression_List* param);

struct S_Expression_Op2* S_CreateExpressionOp2(struct S_Interpreter* interpreter, 
                                               int op, 
                                               struct S_Expression* exp1, 
                                               struct S_Expression* exp2);


//// End of Expression ////

//// Expression List ////
struct S_Expression_List {
    struct S_Expression_List* next;
    struct S_Expression* exp;
};

// Create an expression list. parameter exp could be NULL.
// Create an empty expression list if exp is NULL.
struct S_Expression_List* S_CreateExpList(struct S_Interpreter* interpreter, struct S_Expression* exp);

// Add an expression into expression list.
// exp could not be NULL.
void S_AddExpressionToExpList(struct S_Interpreter* interpreter, struct S_Expression_List* exp_list, struct S_Expression* exp);

//// End of Expression List ////

//// Parameter List ////
struct S_Parameter_List {
    struct S_Parameter_List* next;
    struct S_Expression_Symbol* symbol;
};

// Create an parameter list. param could be NULL.
// Create an empty parameter list if param is NULL.
struct S_Parameter_List* S_CreateParamList(struct S_Interpreter* interpreter, struct S_Expression_Symbol* param);

// Add an parameter into param list.
// param could not be NULL.
void S_AddParameterToParamList(struct S_Interpreter* interpreter, struct S_Parameter_List* param_list, struct S_Expression_Symbol* param);

//// End of Parameter List ////
