%code requires 
{
	#include "S_Interpreter.h"
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

	struct S_Expression*    expression;
    struct S_Statement*     statement;	
    struct S_StatementList* statement_list;
    struct S_CodeBlock*     code_block;
}

%token<int_number>    INTEGER
%token<double_number> DOUBLE
%token<id>       SYMBOL
%token<string>   STRING
%token<cmd_type> CMP
%token RETURN GLOBAL FUNCTION WHILE IF ELSE

%left  CMP
%right '='
%left  '+' '-'
%left  '*' '/'
%right '^'

%type<expression> expr cmp assign add_sub mul_div factor term
%type<statement> stat
%type<statement_list> stat_list
%type<code_block> block

%start stat_list

%%

block : '{' stat_list '}' {}
      ;

stat_list : stat_list stat {}
          | stat {}
          ;

stat : expr ';' {}
     | GLOBAL SYMBOL ';' {}
     | RETURN expr ';' {}
     | FUNCTION SYMBOL block {}
     | WHILE '(' expr ')' block {}
     | IF '(' expr ')' block {}
     | IF '(' expr ')' block ELSE block {}
     ;

expr : cmp {}
     | FUNCTION block {}
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

factor : factor '^' term {}
       | term {}
       ;

term : INTEGER {}
     | DOUBLE  {}
     | SYMBOL  {}
     | STRING  {}
     | '(' expr ')' {}
     | '-' term {}
     ;
%%


