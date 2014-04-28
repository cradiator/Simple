%code requires 
{
	#include "S_Interpreter.h"
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

  struct S_Expression*     expression;
  struct S_Statement*      statement;	
  struct S_StatementList*  statement_list;
  struct S_CodeBlock*      code_block;
  struct S_ExpressionList* expression_list;
  struct S_ParameterList*  parameter_list;
}

%token<int_number>    INTEGER
%token<double_number> DOUBLE
%token<id>       SYMBOL
%token<string>   STRING
%token<cmd_type> CMP
%token RETURN GLOBAL FUNCTION WHILE IF ELSE NIL

%left  CMP
%right '='
%left  '+' '-'
%left  '*' '/'
%right '^'

%type<expression> expr cmp assign add_sub mul_div factor func_call term 
%type<statement> stat
%type<statement_list> stat_list
%type<code_block> block
%type<parameter_list> param_list
%type<expression_list> expr_list

%start stat_list

%%

block : '{' stat_list '}' {}
      ;

stat_list : stat_list stat {}
          | stat {}
          ;

param_list : param_list ',' SYMBOL {}
           | SYMBOL {}
           | {}
           ;

expr_list : expr_list ',' expr {}
          | expr {}
          | {}
          ;

stat : expr ';' {}
     | GLOBAL SYMBOL ';' {}
     | RETURN expr ';' {}
     | FUNCTION SYMBOL '(' param_list ')' block {}
     | WHILE '(' expr ')' block {}
     | IF '(' expr ')' block {}
     | IF '(' expr ')' block ELSE block {}
     ;

expr : cmp {}
     | FUNCTION '(' param_list ')' block {}
     ;

cmp : cmp CMP assign {}
    | assign {}
    ;

assign : assign '=' add_sub {}
       | add_sub {}
       ;

add_sub : add_sub '+' mul_div {}
        | add_sub '-' mul_div {}
        | mul_div {}
        ;

mul_div : mul_div '*' factor {}
        | mul_div '/' factor {}
        | factor {}
        ;

factor : factor '^' func_call {}
       | func_call {}
       ;

func_call : term '(' expr_list ')' {}
          | term {}
          ;

term : INTEGER {}
     | DOUBLE  {}
     | SYMBOL  {}
     | STRING  {}
     | NULL {}
     | '(' expr ')' {}
     | '-' term {}
     ;
%%


