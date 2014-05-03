%{
#include "Common.h"
#include "S_Interpreter.h"
#include "S_String.h"
#include "simple.tab.h"

#define YY_DECL int yylex(struct S_Interpreter* interpreter)
#define YY_ALWAYS_INTERACTIVE 1
%}

%option noyywrap nodefault interactive yylineno
%start STRING_CONTENT COMMENT

%%
[0-9]+ {yylloc = S_GetSrcLineNo(interpreter); yylval.int_number = atoi(yytext); return INTEGER;}

\.[0-9]+ |
[0-9]+\.[0-9]* {yylloc = S_GetSrcLineNo(interpreter); yylval.double_number = atof(yytext); return DOUBLE;}

"+"  |
"-"  |
"*"  |
"/"  |
"("  |
")"  |
"{"  |
"}"  |
"="  |
";"  |
","  |
"^"  {yylloc = S_GetSrcLineNo(interpreter); return yytext[0];}

">"  {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_G; return CMP;}
">=" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_GE; return CMP;}
"==" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_E; return CMP;}
"<"  {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_L; return CMP;}
"<=" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_LE; return CMP;}
"!=" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_NE; return CMP;}

"&&" {yylloc = S_GetSrcLineNo(interpreter); yylval.rel_type = OP2_AND; return REL;}
"||" {yylloc = S_GetSrcLineNo(interpreter); yylval.rel_type = OP2_OR; return REL;}

"function" {yylloc = S_GetSrcLineNo(interpreter); return FUNCTION;}

"global"   {yylloc = S_GetSrcLineNo(interpreter); return GLOBAL;}

"return"   {yylloc = S_GetSrcLineNo(interpreter); return RETURN;}

"while"    {yylloc = S_GetSrcLineNo(interpreter); return WHILE;}

"if"       {yylloc = S_GetSrcLineNo(interpreter); return IF;}

"else"     {yylloc = S_GetSrcLineNo(interpreter); return ELSE;}

"nil"      {yylloc = S_GetSrcLineNo(interpreter); return NIL;}

"true"     {yylloc = S_GetSrcLineNo(interpreter); return TRUE;}

"false"    {yylloc = S_GetSrcLineNo(interpreter); return FALSE;}

[a-zA-Z]   {yylloc = S_GetSrcLineNo(interpreter); yylval.id = S_AllocateStringInParsingStorage(interpreter, yytext); return SYMBOL;}

\n         {S_IncSrcLineNo(interpreter);}

[ \t]      {}

\"                   {yylloc = S_GetSrcLineNo(interpreter); S_OpenString(interpreter); BEGIN STRING_CONTENT;}
<STRING_CONTENT>\"   {yylval.string = S_CloseString(interpreter); BEGIN INITIAL; return STRING;}
<STRING_CONTENT>\n   {S_AddStringChar(interpreter, '\n'); S_IncSrcLineNo(interpreter);}
<STRING_CONTENT>\\\" {S_AddStringChar(interpreter, '\"');} 
<STRING_CONTENT>\\t  {S_AddStringChar(interpreter, '\t');}
<STRING_CONTENT>\\n  {S_AddStringChar(interpreter, '\n');}
<STRING_CONTENT>\\\\ {S_AddStringChar(interpreter, '\\');}
<STRING_CONTENT>.    {S_AddStringChar(interpreter, yytext[0]);}

"//" {BEGIN COMMENT;}
"#"  {BEGIN COMMENT;}
<COMMENT>\n {S_IncSrcLineNo(interpreter); BEGIN INITIAL;}
<COMMENT>.  {}

.          {}

%%

