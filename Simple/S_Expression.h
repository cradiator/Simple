#pragma once

enum {
    EXPRESSION_TYPE_NULL = 0,
    EXPRESSION_TYPE_INTEGER,
    EXPRESSION_TYPE_DOUBLE,
    EXPRESSION_TYPE_STRING,
    EXPRESSION_TYPE_op1,
    EXPRESSION_TYPE_op2,
}

struct S_Expression_Header {
    int type;
};

struct S_Expresstion {
    struct S_Expression_Header header;
};

struct S_Expression_Null {
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

struct S_Expression_Op1 {
    struct S_Expression_Header header;
    struct S_Expression *exp;
    int op;
};

struct S_Expression_Op2 {
    struct S_Expression_Header header;
    struct S_Expression *exp1;
    struct S_Expression *exp2;
    int op;
};

