%{
#include "Common.h"
#include "S_Interpreter.h"
#include "S_String.h"
#include "simple.tab.h"

#define YY_DECL int yylex(struct S_Interpreter* interpreter)
#define YY_ALWAYS_INTERACTIVE 1
%}

%option noyywrap nodefault interactive yylineno
%start STRING_CONTENT CHAR_CONTENT COMMENT

%%
<INITIAL>[0-9]+ {yylloc = S_GetSrcLineNo(interpreter); yylval.int_number = atoi(yytext); return INTEGER;}

<INITIAL>\.[0-9]+ |
<INITIAL>[0-9]+\.[0-9]* {yylloc = S_GetSrcLineNo(interpreter); yylval.double_number = atof(yytext); return DOUBLE;}

<INITIAL>"+"  |
<INITIAL>"-"  |
<INITIAL>"*"  |
<INITIAL>"/"  |
<INITIAL>"("  |
<INITIAL>")"  |
<INITIAL>"{"  |
<INITIAL>"}"  |
<INITIAL>"="  |
<INITIAL>";"  |
<INITIAL>","  |
<INITIAL>"^"  |
<INITIAL>"["  |
<INITIAL>"]"  {yylloc = S_GetSrcLineNo(interpreter); return yytext[0];}

<INITIAL>\.   {yylloc = S_GetSrcLineNo(interpreter); return DOT;}

<INITIAL>">"  {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_G; return CMP;}
<INITIAL>">=" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_GE; return CMP;}
<INITIAL>"==" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_E; return CMP;}
<INITIAL>"<"  {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_L; return CMP;}
<INITIAL>"<=" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_LE; return CMP;}
<INITIAL>"!=" {yylloc = S_GetSrcLineNo(interpreter); yylval.cmp_type = OP2_NE; return CMP;}

<INITIAL>"&&" {yylloc = S_GetSrcLineNo(interpreter); yylval.rel_type = OP2_AND; return REL;}
<INITIAL>"||" {yylloc = S_GetSrcLineNo(interpreter); yylval.rel_type = OP2_OR; return REL;}

<INITIAL>"function" {yylloc = S_GetSrcLineNo(interpreter); return FUNCTION;}

<INITIAL>"global"   {yylloc = S_GetSrcLineNo(interpreter); return GLOBAL;}

<INITIAL>"return"   {yylloc = S_GetSrcLineNo(interpreter); return RETURN;}

<INITIAL>"while"    {yylloc = S_GetSrcLineNo(interpreter); return WHILE;}

<INITIAL>"if"       {yylloc = S_GetSrcLineNo(interpreter); return IF;}

<INITIAL>"else"     {yylloc = S_GetSrcLineNo(interpreter); return ELSE;}

<INITIAL>"elif"     {yylloc = S_GetSrcLineNo(interpreter); return ELIF;}

<INITIAL>"nil"      {yylloc = S_GetSrcLineNo(interpreter); return NIL;}

<INITIAL>"true"     {yylloc = S_GetSrcLineNo(interpreter); return TRUE;}

<INITIAL>"false"    {yylloc = S_GetSrcLineNo(interpreter); return FALSE;}


<INITIAL>[a-zA-Z_][a-zA-Z0-9_]*   {yylloc = S_GetSrcLineNo(interpreter); yylval.id = S_AllocateStringInParsingStorage(interpreter, yytext); return SYMBOL;}

<INITIAL>\n         {S_IncSrcLineNo(interpreter);}

<INITIAL>[ \t\r]    {}

<INITIAL>\"          {yylloc = S_GetSrcLineNo(interpreter); S_OpenString(interpreter); BEGIN STRING_CONTENT;}
<STRING_CONTENT>\"   {yylval.string = S_CloseString(interpreter); BEGIN INITIAL; return STRING;}
<STRING_CONTENT>\n   {S_AddStringChar(interpreter, '\n'); S_IncSrcLineNo(interpreter);}
<STRING_CONTENT>\\\" {S_AddStringChar(interpreter, '\"');} 
<STRING_CONTENT>\\t  {S_AddStringChar(interpreter, '\t');}
<STRING_CONTENT>\\n  {S_AddStringChar(interpreter, '\n');}
<STRING_CONTENT>\\\\ {S_AddStringChar(interpreter, '\\');}
<STRING_CONTENT>.    {S_AddStringChar(interpreter, yytext[0]);}
<STRING_CONTENT><<EOF>> {yyerror(interpreter, "Unclosing quote"); return ERROR_LEXICAL_MARK;}

<INITIAL>'\\n'   {yylloc = S_GetSrcLineNo(interpreter); yylval.char_ascii = '\n'; return CHAR;}
<INITIAL>'\\t'   {yylloc = S_GetSrcLineNo(interpreter); yylval.char_ascii = '\t'; return CHAR;}
<INITIAL>'\\''   {yylloc = S_GetSrcLineNo(interpreter); yylval.char_ascii = '\''; return CHAR;}
<INITIAL>'\\\\'  {yylloc = S_GetSrcLineNo(interpreter); yylval.char_ascii = '\\'; return CHAR;}
<INITIAL>'.'     {yylloc = S_GetSrcLineNo(interpreter); yylval.char_ascii = yytext[1]; return CHAR;}


<INITIAL>"//" {BEGIN COMMENT;}
<INITIAL>"#"  {BEGIN COMMENT;}
<COMMENT>\n {S_IncSrcLineNo(interpreter); BEGIN INITIAL;}
<COMMENT>.  {}
<COMMENT><<EOF>> {return -1;}

<INITIAL><<EOF>>   {return -1;}
<INITIAL>.         {yyerror(interpreter, "Invalid lexical mark 0x%x.", yytext[0]); return ERROR_LEXICAL_MARK;}

%%

