%code requires 
{
	#include "S_Interpreter.h"
  #include "S_Expression.h"
  #define YYLTYPE int;
}

%defines
%locations
// %define api.pure full
%parse-param {struct S_Interpreter* interpreter}
%lex-param   {struct S_Interpreter* interpreter}

%union {
  int    int_number;
  double double_number;	
  char*  id;
  char*  string;
  int    cmp_type;
  int    rel_type;

  struct S_Expression*      expression;
  struct S_Statement*       statement;	
  struct S_Statement_List*  statement_list;
  struct S_Code_Block*      code_block;
  struct S_Expression_List* expression_list;
  struct S_Parameter_List*  parameter_list;
}

%token<int_number>    INTEGER
%token<double_number> DOUBLE
%token<id>       SYMBOL
%token<string>   STRING
%token<cmd_type> CMP
%token<rel_type> REL
%token RETURN GLOBAL FUNCTION WHILE IF ELSE NIL TRUE FALSE

%left  REL
%left  CMP
%right '='
%left  '+' '-'
%left  '*' '/'
%right '^'

%type<expression> expr cmp rel assign add_sub mul_div factor func_call term 
%type<statement> stat
%type<statement_list> stat_list
%type<code_block> block
%type<parameter_list> param_list
%type<expression_list> expr_list

%start stat_list

%%

block : '{' stat_list '}' {$$ = S_CreateCodeBlock(interpreter, $2);}
      | '{' '}' {$$ = S_CreateCodeBlock(interpreter, S_CreateStatementList(interpreter, 0));}
      ;

stat_list : stat_list stat {S_AddStatementToStatList(interpreter, $1, $2); $$ = $1;}
          | stat {$$ = S_CreateStatementList(interpreter, $1);}
          ;

param_list : param_list ',' SYMBOL {S_AddParameterToParamList(interpreter, $1, $3); $$ = $1;}
           | SYMBOL {$$ = S_CreateParamList(interpreter, $1);}
           | {$$ = S_CreateParamList(interpreter, 0);}
           ;

expr_list : expr_list ',' expr {S_AddExpressionToExpList(interpreter, $1, $3); $$ = $1;}
          | expr {$$ = S_CreateExpList(interpreter, $1);}
          | {$$ = S_CreateExpList(interpreter, 0);}
          ;

stat : expr ';' {$$ = (struct S_Statement*)S_CreateStatementExpression(interpreter, $1);}
     | GLOBAL SYMBOL ';' {$$ = (struct S_Statement*)S_CreateStatementGlobal(interpreter, $2);}
     | RETURN expr ';' {$$ = (struct S_Statement*)S_CreateStatementReturn(interpreter, $2);}
     | FUNCTION SYMBOL '(' param_list ')' block {$$ = (struct S_Statement*)S_CreateStatementFunctionDefine(interpreter, $2, $4, $6);}
     | WHILE '(' expr ')' block {$$ = (struct S_Statement*)S_CreateStatementWhile(interpreter, $3, $5);}
     | IF '(' expr ')' block {$$ = (struct S_Statement*)S_CreateStatementIf(interpreter, $3, $5, 0);}
     | IF '(' expr ')' block ELSE block {$$ = (struct S_Statement*)S_CreateStatementIf(interpreter, $3, $5, $7);}
     ;

expr : assign {$$ = $1;}
     | FUNCTION '(' param_list ')' block {$$ = S_CreateExpressionFunctionDefine(interpreter, $3, $5);}
     ;

assign : assign '=' rel {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ASSIGN, $1, $3);}
       | rel {$$ = $1;}
       ;

rel : rel REL cmp {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, $2, $1, $3);}
    | cmp {$$ = $1;}
    ;

cmp : cmp CMP add_sub {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, $2, $1, $3);}
    | add_sub {$$ = $1;}
    ;

add_sub : add_sub '+' mul_div {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ADD, $1, $3);}
        | add_sub '-' mul_div {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, OP2_SUB, $1, $3);}
        | mul_div {$$ = $1;}
        ;

mul_div : mul_div '*' factor {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, OP2_MUL, $1, $3);}
        | mul_div '/' factor {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, OP2_DIV, $1, $3);}
        | factor {$$ = $1;}
        ;

factor : factor '^' func_call {$$ = (S_Expression*)S_CreateExpressionOp2(interpreter, OP2_FACTOR, $1, $3);}
       | func_call {$$ = $1;}
       ;

func_call : term '(' expr_list ')' {$$ = (S_Expression*)S_CreateExpressionFunctionCall(interpreter, $1, $3);}
          | term {$$ = $1;}
          ;

term : INTEGER {$$ = (S_Expression*)S_CreateExpressionInteger(interpreter, $1);}
     | DOUBLE  {$$ = (S_Expression*)S_CreateExpressionDouble(interpreter, $1);}
     | SYMBOL  {$$ = (S_Expression*)S_CreateExpressionSymbol(interpreter, $1);}
     | STRING  {$$ = (S_Expression*)S_CreateExpressionString(interpreter, $1);}
     | NIL {$$ = (S_Expression*)S_CreateExpressionNil(interpreter);}
     | TRUE  {$$ = (S_Expression*)S_CreateExpressionTrue(interpreter);}
     | FALSE {$$ = (S_Expression*)S_CreateExpressionFalse(interpreter);}
     | '(' expr ')' {$$ = $2;}
     | '-' term {$$ = (S_Expression*)S_CreateExpressionNegation(interpreter, $2);}
     ;
%%


