%code requires 
{
	#include "S_Interpreter.h"
  #include "S_Expression.h"

  #define YYLTYPE_IS_DECLARED 1
  # define YYLLOC_DEFAULT(Current, Rhs, N)  {Current = YYRHSLOC(Rhs, N);}
  typedef int YYLTYPE;

  int yylex(struct S_Interpreter*);
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
  char   char_ascii;
  int    cmp_type;
  int    rel_type;

  struct S_Expression*      expression;
  struct S_Statement*       statement;	
  struct S_Statement_List*  statement_list;
  struct S_Code_Block*      code_block;
  struct S_Expression_List* expression_list;
  struct S_Parameter_List*  parameter_list;
  struct S_Elif_List*       elif_list;
}

%token<int_number>    INTEGER
%token<double_number> DOUBLE
%token<id>       SYMBOL
%token<string>   STRING
%token<char_ascii> CHAR
%token<cmp_type> CMP
%token<rel_type> REL
%token RETURN GLOBAL FUNCTION WHILE IF ELSE ELIF NIL TRUE FALSE DOT BREAK CONTINUE FOR
%token ERROR_LEXICAL_MARK

%left '.'
%left  REL
%left  CMP
%right '='
%left  '+' '-'
%left  '*' '/'
%right '^'

%type<expression> expr cmp rel func_def assign add_sub mul_div factor func_call term postfix expr_or_empty
%type<statement> stat
%type<statement_list> stat_list start
%type<code_block> block
%type<parameter_list> param_list
%type<expression_list> expr_list
%type<elif_list> else_if_list

%start start

%%
start : stat_list {interpreter->StatementList = $1; $$ = $1;}
      ;

block : '{' stat_list '}' {$$ = S_CreateCodeBlock(interpreter, $2);}
      | '{' '}' {$$ = S_CreateCodeBlock(interpreter, S_CreateStatementList(interpreter, 0));}
      ;

stat_list : stat_list stat {S_AddStatementToStatList(interpreter, $1, $2); $$ = $1;}
          | stat {$$ = S_CreateStatementList(interpreter, $1);}
          ;

param_list : param_list ',' SYMBOL {S_AddParameterToParamList(interpreter, $1, S_CreateExpressionSymbol(interpreter, $3)); $$ = $1;}
           | SYMBOL {$$ = S_CreateParamList(interpreter, S_CreateExpressionSymbol(interpreter, $1));}
           | {$$ = S_CreateParamList(interpreter, 0);}
           ;

expr_list : expr_list ',' expr {S_AddExpressionToExpList(interpreter, $1, $3); $$ = $1;}
          | expr {$$ = S_CreateExpList(interpreter, $1);}
          | {$$ = S_CreateExpList(interpreter, 0);}
          ;

else_if_list : else_if_list ELIF '(' expr ')'block {S_AddElifList(interpreter, $1, $4, $6); $$ = $1;}
             | ELIF '(' expr ')'block {$$ = S_CreateElifList(interpreter, $3, $5);}
             ;

expr_or_empty : expr {$$ = $1;}
              | {$$ = 0;}
              ;

stat : expr ';' {$$ = (struct S_Statement*)S_CreateStatementExpression(interpreter, $1);}
     | GLOBAL SYMBOL ';' {$$ = (struct S_Statement*)S_CreateStatementGlobal(interpreter, S_CreateExpressionSymbol(interpreter, $2));}
     | RETURN expr ';' {$$ = (struct S_Statement*)S_CreateStatementReturn(interpreter, $2);}
     | RETURN ';' {$$ = (struct S_Statement*)S_CreateStatementReturn(interpreter, (struct S_Expression*)S_CreateExpressionNil(interpreter));}
     | FUNCTION SYMBOL '(' param_list ')' block {$$ = (struct S_Statement*)S_CreateStatementFunctionDefine(interpreter, S_CreateExpressionSymbol(interpreter, $2), $4, $6);}
     | WHILE '(' expr ')' block {$$ = (struct S_Statement*)S_CreateStatementWhile(interpreter, $3, $5);}
     | FOR '(' expr_or_empty ';' expr_or_empty ';' expr_or_empty ')' block {$$ = (struct S_Statement*)S_CreateStatementFor(interpreter, $3, $5, $7, $9);}
     | IF '(' expr ')' block {$$ = (struct S_Statement*)S_CreateStatementIf(interpreter, $3, $5, 0, 0);}
     | IF '(' expr ')' block ELSE block {$$ = (struct S_Statement*)S_CreateStatementIf(interpreter, $3, $5, 0, $7);}
     | IF '(' expr ')' block else_if_list {$$ = (struct S_Statement*)S_CreateStatementIf(interpreter, $3, $5, $6, 0);}
     | IF '(' expr ')' block else_if_list ELSE block {$$ = (struct S_Statement*)S_CreateStatementIf(interpreter, $3, $5, $6, $8);}
     | BREAK ';' {$$ = (struct S_Statement*)S_CreateStatementBreak(interpreter);}
     | CONTINUE ';' {$$ = (struct S_Statement*)S_CreateStatementContinue(interpreter);}
     ;

expr : assign {$$ = $1;}
     ;

assign : assign '=' func_def {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ASSIGN, $1, $3);}
       | assign '=' rel {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ASSIGN, $1, $3);}
       | func_def {$$ = $1;}
       | rel {$$ = $1;}
       ;

func_def : FUNCTION '(' param_list ')' block {$$ = (struct S_Expression*)S_CreateExpressionFunctionDefine(interpreter, $3, $5);}
         ;

rel : rel REL cmp {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, $2, $1, $3);}
    | cmp {$$ = $1;}
    ;

cmp : cmp CMP add_sub {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, $2, $1, $3);}
    | add_sub {$$ = $1;}
    ;

add_sub : add_sub '+' mul_div {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ADD, $1, $3);}
        | add_sub '-' mul_div {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_SUB, $1, $3);}
        | mul_div {$$ = $1;}
        ;

mul_div : mul_div '*' factor {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_MUL, $1, $3);}
        | mul_div '/' factor {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_DIV, $1, $3);}
        | factor {$$ = $1;}
        ;

factor : factor '^' func_call {$$ = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_FACTOR, $1, $3);}
       | func_call {$$ = $1;}
       ;

func_call : func_def '(' expr_list ')' {$$ = (struct S_Expression*)S_CreateExpressionFunctionCall(interpreter, $1, $3);}
          | func_call '(' expr_list ')' {$$ = (struct S_Expression*)S_CreateExpressionFunctionCall(interpreter, $1, $3);}
          | postfix {$$ = $1;}
          ;


postfix : term {$$ = $1;}
        | postfix '[' expr ']' {$$ = (struct S_Expression*)S_CreateExpressionSubscript(interpreter, $1, $3);}
        | postfix DOT SYMBOL {$$ = (struct S_Expression*)S_CreateExpressionDot(interpreter, $1, S_CreateExpressionSymbol(interpreter, $3));}
        ;

term : INTEGER {$$ = (struct S_Expression*)S_CreateExpressionInteger(interpreter, $1);}
     | DOUBLE  {$$ = (struct S_Expression*)S_CreateExpressionDouble(interpreter, $1);}
     | SYMBOL  {$$ = (struct S_Expression*)S_CreateExpressionSymbol(interpreter, $1);}
     | STRING  {$$ = (struct S_Expression*)S_CreateExpressionString(interpreter, $1);}
     | NIL {$$ = (struct S_Expression*)S_CreateExpressionNil(interpreter);}
     | TRUE  {$$ = (struct S_Expression*)S_CreateExpressionTrue(interpreter);}
     | FALSE {$$ = (struct S_Expression*)S_CreateExpressionFalse(interpreter);}
     | CHAR  {$$ = (struct S_Expression*)S_CreateExpressionChar(interpreter, $1);}
     | '(' expr ')' {$$ = $2;}
     | '-' term {$$ = (struct S_Expression*)S_CreateExpressionNegation(interpreter, $2);}
     | '[' expr_list ']' {$$ = (struct S_Expression*)S_CreateExpressionArray(interpreter, $2);}
     | ERROR_LEXICAL_MARK {YYABORT;}
     ;
%%


