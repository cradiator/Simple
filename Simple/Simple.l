%option noyywrap nodefault interactive yylineno

%%
[0-9]+ {lvalp->int_number = atoi(yytext); return INTEGER;}

\.[0-9]+ |
[0-9]+\.[0-9]* {lvalp->double_number = atof(yytext); return DOUBLE;}

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
"^"  {return yytext[0];}

"function" {return FUNCTION;}

"global"   {return GLOBAL;}

"return"   {return RETURN;}

"while"    {return WHILE;}

"if"       {return IF;}

"else"     {return ELSE;}

[a-zA-Z]   {return SYMBOL;}



%%

