#pragma once

//// Declearation  ////
struct S_Expression_List;
struct S_Parameter_List;
struct S_Statement_List;
struct S_Code_Block;
struct S_Elif_List;
struct S_Expression_Header;

//// End od Declearation ////

//// Expression /////
enum S_Expression_Type {
    EXPRESSION_TYPE_NIL = 0,
    EXPRESSION_TYPE_INTEGER,
    EXPRESSION_TYPE_DOUBLE,
    EXPRESSION_TYPE_TRUE,
    EXPRESSION_TYPE_FALSE,
    EXPRESSION_TYPE_CHAR,
    EXPRESSION_TYPE_STRING,
    EXPRESSION_TYPE_SYMBOL,
    EXPRESSION_TYPE_NEGATION,
    EXPRESSION_TYPE_FUNCTION_CALL,
    EXPRESSION_TYPE_FUNCTION_DEFINE,
    EXPRESSION_TYPE_OP2,
    EXPRESSION_TYPE_ARRAY,
    EXPRESSION_TYPE_SUBSCRIPT,
    EXPRESSION_TYPE_DOT,
};

enum {
    OP2_MIN = 0,
    OP2_ASSIGN = OP2_MIN,
    OP2_AND,
    OP2_OR,
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
    enum S_Expression_Type type;
    int lineno;
};

struct S_Expression {
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

struct S_Expression_True {
    struct S_Expression_Header header;
};

struct S_Expression_False {
    struct S_Expression_Header header;
};

struct S_Expression_Char {
    struct S_Expression_Header header;
    char c;
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

struct S_Expression_Function_Define {
    struct S_Expression_Header header;
    struct S_Parameter_List* param;
    struct S_Code_Block* code;
};

struct S_Expression_Op2 {
    struct S_Expression_Header header;
    struct S_Expression *exp1;
    struct S_Expression *exp2;
    int op;
};

struct S_Expression_Array {
    struct S_Expression_Header header;
    struct S_Expression_List* exp_list;
};

struct S_Expression_Subscript {
    struct S_Expression_Header header;
    struct S_Expression* instance;
    struct S_Expression* index;
};

struct S_Expression_Dot {
    struct S_Expression_Header header;
    struct S_Expression* instance;
    struct S_Expression_Symbol* field;
};


EXTERN_C struct S_Expression_Nil* S_CreateExpressionNil(struct S_Interpreter* interpreter);

EXTERN_C struct S_Expression_Integer* S_CreateExpressionInteger(struct S_Interpreter* interpreter, int value);

EXTERN_C struct S_Expression_Double* S_CreateExpressionDouble(struct S_Interpreter* interpreter, double value);

EXTERN_C struct S_Expression_True* S_CreateExpressionTrue(struct S_Interpreter* interpreter);

EXTERN_C struct S_Expression_False* S_CreateExpressionFalse(struct S_Interpreter* interpreter);

// This function would take ownership of string.
EXTERN_C struct S_Expression_String* S_CreateExpressionString(struct S_Interpreter* interpreter, char* string);

EXTERN_C struct S_Expression_Char* S_CreateExpressionChar(struct S_Interpreter* interpreter, char c);

// This function would take ownership of symbol.
EXTERN_C struct S_Expression_Symbol* S_CreateExpressionSymbol(struct S_Interpreter* interpreter, char* symbol);

EXTERN_C struct S_Expression_Negation* S_CreateExpressionNegation(struct S_Interpreter* interpreter, struct S_Expression* exp);

EXTERN_C struct S_Expression_Function_Call* S_CreateExpressionFunctionCall(struct S_Interpreter* interpreter,
                                                                  struct S_Expression *fn,
                                                                  struct S_Expression_List* param);

EXTERN_C struct S_Expression_Function_Define* S_CreateExpressionFunctionDefine(struct S_Interpreter* interpreter,
                                                                      struct S_Parameter_List* param,
                                                                      struct S_Code_Block* code);

EXTERN_C struct S_Expression_Op2* S_CreateExpressionOp2(struct S_Interpreter* interpreter, 
                                               int op, 
                                               struct S_Expression* exp1, 
                                               struct S_Expression* exp2);

EXTERN_C struct S_Expression_Array* S_CreateExpressionArray(struct S_Interpreter* interpreter,
                                                            struct S_Expression_List* exp_list);

EXTERN_C struct S_Expression_Subscript* S_CreateExpressionSubscript(struct S_Interpreter* interpreter,
                                                                    struct S_Expression* instance,
                                                                    struct S_Expression* index);

EXTERN_C struct S_Expression_Dot* S_CreateExpressionDot(struct S_Interpreter* interpreter,
                                                        struct S_Expression* instance,
                                                        struct S_Expression_Symbol* field);

//// End of Expression ////

//// Expression List ////
struct S_Expression_List {
    struct S_Expression_List* next;
    struct S_Expression* exp;
};

// Create an expression list. parameter exp could be NULL.
// Create an empty expression list if exp is NULL.
EXTERN_C struct S_Expression_List* S_CreateExpList(struct S_Interpreter* interpreter, struct S_Expression* exp);

// Add an expression into expression list.
// exp could not be NULL.
EXTERN_C void S_AddExpressionToExpList(struct S_Interpreter* interpreter, struct S_Expression_List* exp_list, struct S_Expression* exp);

//// End of Expression List ////

//// Parameter List ////
struct S_Parameter_List {
    struct S_Parameter_List* next;
    struct S_Expression_Symbol* symbol;
};

// Create an parameter list. param could be NULL.
// Create an empty parameter list if param is NULL.
EXTERN_C struct S_Parameter_List* S_CreateParamList(struct S_Interpreter* interpreter, struct S_Expression_Symbol* param);

// Add an parameter into param list.
// param could not be NULL.
EXTERN_C void S_AddParameterToParamList(struct S_Interpreter* interpreter, struct S_Parameter_List* param_list, struct S_Expression_Symbol* param);

//// End of Parameter List ////

//// Statement ////
enum {
    STATEMENT_TYPE_EXPRESSION,
    STATEMENT_TYPE_GLOBAL,
    STATEMENT_TYPE_RETURN,
    STATEMENT_TYPE_FUNCTION_DEFINE,
    STATEMENT_TYPE_WHILE,
    STATEMENT_TYPE_IF,
};

struct S_Statement_Header {
    int type;
};

struct S_Statement {
    struct S_Statement_Header header;
};

struct S_Statement_Expression {
    struct S_Statement_Header header;
    struct S_Expression* exp;
};

struct S_Statement_Global {
    struct S_Statement_Header header;
    struct S_Expression_Symbol* symbol;
};

struct S_Statement_Return {
    struct S_Statement_Header header;
    struct S_Expression* exp;
};

struct S_Statement_Function_Define {
    struct S_Statement_Header   header;
    struct S_Expression_Symbol* name;
    struct S_Parameter_List*    param;
    struct S_Code_Block*        code;
};

struct S_Statement_While {
    struct S_Statement_Header header;
    struct S_Expression* condition;
    struct S_Code_Block* body;
};

struct S_Statement_If {
    struct S_Statement_Header header;
    struct S_Expression* condition;
    struct S_Code_Block* body;
    struct S_Elif_List*  elif_list;
    struct S_Code_Block* else_body;
};

EXTERN_C struct S_Statement_Expression* S_CreateStatementExpression(struct S_Interpreter* interpreter, 
                                                           struct S_Expression* expression);

EXTERN_C struct S_Statement_Global* S_CreateStatementGlobal(struct S_Interpreter* interpreter,
                                                   struct S_Expression_Symbol* symbol);

EXTERN_C struct S_Statement_Return* S_CreateStatementReturn(struct S_Interpreter* interpreter, 
                                                   struct S_Expression* expression);

EXTERN_C struct S_Statement_Function_Define* S_CreateStatementFunctionDefine(struct S_Interpreter* interpreter,
                                                                    struct S_Expression_Symbol*  name,
                                                                    struct S_Parameter_List* param,
                                                                    struct S_Code_Block* code);

EXTERN_C struct S_Statement_While* S_CreateStatementWhile(struct S_Interpreter* interpreter,
                                                 struct S_Expression* condition,
                                                 struct S_Code_Block* body);

// else_body could be NULL
EXTERN_C struct S_Statement_If* S_CreateStatementIf(struct S_Interpreter* interpreter,
                                           struct S_Expression* condition,
                                           struct S_Code_Block* body,
                                           struct S_Elif_List*  elif_list,
                                           struct S_Code_Block* else_body);

//// End of Statement ////

//// Statement List ////
struct S_Statement_List {
    struct S_Statement_List* next;
    struct S_Statement*      stat;
};

// Create a statment list.
// The parameter statement could be NULL, and would create an empty list.
EXTERN_C struct S_Statement_List* S_CreateStatementList(struct S_Interpreter* interpreter, 
                                               struct S_Statement* statement);

// Add a statement to stat-list.
// statement could not be NULL.
EXTERN_C void S_AddStatementToStatList(struct S_Interpreter* interpreter,
                              struct S_Statement_List* stat_list,
                              struct S_Statement* statement);

//// End of Statement List ////

//// Code Block ////
struct S_Code_Block {
    struct S_Statement_List* stat_list;
};

// Create an code block.
// stat_list could be NULL.
EXTERN_C struct S_Code_Block* S_CreateCodeBlock(struct S_Interpreter* interpreter,
                                       struct S_Statement_List* stat_list);

//// End of Clode Block ////

//// else if list ////
struct S_Elif_List {
    struct S_Elif_List* next;
    struct S_Expression* condition;
    struct S_Code_Block* body;
};

EXTERN_C struct S_Elif_List* S_CreateElifList(struct S_Interpreter* interpreter,
                                              struct S_Expression* condition,
                                              struct S_Code_Block* body);

EXTERN_C void S_AddElifList(struct S_Interpreter* interpreter,
                            struct S_Elif_List* elif_list,
                            struct S_Expression* condition,
                            struct S_Code_Block* body);


//// else if list ////

