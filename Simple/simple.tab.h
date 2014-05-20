
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 2 "simple.y"

	#include "S_Interpreter.h"
  #include "S_Expression.h"

  #define YYLTYPE_IS_DECLARED 1
  # define YYLLOC_DEFAULT(Current, Rhs, N)  {Current = YYRHSLOC(Rhs, N);}
  typedef int YYLTYPE;

  int yylex(struct S_Interpreter*);



/* Line 1676 of yacc.c  */
#line 53 "simple.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     DOUBLE = 259,
     SYMBOL = 260,
     STRING = 261,
     CMP = 262,
     REL = 263,
     RETURN = 264,
     GLOBAL = 265,
     FUNCTION = 266,
     WHILE = 267,
     IF = 268,
     ELSE = 269,
     ELIF = 270,
     NIL = 271,
     TRUE = 272,
     FALSE = 273,
     ERROR_LEXICAL_MARK = 274
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 19 "simple.y"

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
  struct S_Elif_List*       elif_list;



/* Line 1676 of yacc.c  */
#line 108 "simple.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

