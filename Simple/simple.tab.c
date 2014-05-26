
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */


/* Line 189 of yacc.c  */
#line 73 "simple.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 2 "simple.y"

	#include "S_Interpreter.h"
  #include "S_Expression.h"

  #define YYLTYPE_IS_DECLARED 1
  # define YYLLOC_DEFAULT(Current, Rhs, N)  {Current = YYRHSLOC(Rhs, N);}
  typedef int YYLTYPE;

  int yylex(struct S_Interpreter*);



/* Line 209 of yacc.c  */
#line 110 "simple.tab.c"

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
     CHAR = 262,
     CMP = 263,
     REL = 264,
     RETURN = 265,
     GLOBAL = 266,
     FUNCTION = 267,
     WHILE = 268,
     IF = 269,
     ELSE = 270,
     ELIF = 271,
     NIL = 272,
     TRUE = 273,
     FALSE = 274,
     DOT = 275,
     BREAK = 276,
     CONTINUE = 277,
     ERROR_LEXICAL_MARK = 278
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 19 "simple.y"

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



/* Line 214 of yacc.c  */
#line 170 "simple.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 195 "simple.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  62
/* YYNRULES -- Number of states.  */
#define YYNSTATES  121

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    28,    26,    33,    27,    24,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    36,
       2,    25,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,    32,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     9,    12,    15,    17,    21,    23,
      24,    28,    30,    31,    38,    44,    47,    51,    55,    58,
      65,    71,    77,    85,    92,   101,   104,   107,   109,   113,
     117,   119,   121,   127,   131,   133,   137,   139,   143,   147,
     149,   153,   157,   159,   163,   165,   170,   175,   177,   179,
     184,   188,   190,   192,   194,   196,   198,   200,   202,   204,
     208,   211,   215
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    42,    -1,    31,    42,    32,    -1,    31,
      32,    -1,    42,    46,    -1,    46,    -1,    43,    33,     5,
      -1,     5,    -1,    -1,    44,    33,    47,    -1,    47,    -1,
      -1,    45,    16,    34,    47,    35,    41,    -1,    16,    34,
      47,    35,    41,    -1,    47,    36,    -1,    11,     5,    36,
      -1,    10,    47,    36,    -1,    10,    36,    -1,    12,     5,
      34,    43,    35,    41,    -1,    13,    34,    47,    35,    41,
      -1,    14,    34,    47,    35,    41,    -1,    14,    34,    47,
      35,    41,    15,    41,    -1,    14,    34,    47,    35,    41,
      45,    -1,    14,    34,    47,    35,    41,    45,    15,    41,
      -1,    21,    36,    -1,    22,    36,    -1,    48,    -1,    48,
      25,    49,    -1,    48,    25,    50,    -1,    49,    -1,    50,
      -1,    12,    34,    43,    35,    41,    -1,    50,     9,    51,
      -1,    51,    -1,    51,     8,    52,    -1,    52,    -1,    52,
      26,    53,    -1,    52,    27,    53,    -1,    53,    -1,    53,
      28,    54,    -1,    53,    29,    54,    -1,    54,    -1,    54,
      30,    55,    -1,    55,    -1,    49,    34,    44,    35,    -1,
      55,    34,    44,    35,    -1,    56,    -1,    57,    -1,    56,
      37,    47,    38,    -1,    56,    20,     5,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,    17,    -1,    18,    -1,
      19,    -1,     7,    -1,    34,    47,    35,    -1,    27,    57,
      -1,    37,    44,    38,    -1,    23,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    66,    66,    69,    70,    73,    74,    77,    78,    79,
      82,    83,    84,    87,    88,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   105,   108,   109,
     110,   111,   114,   117,   118,   121,   122,   125,   126,   127,
     130,   131,   132,   135,   136,   139,   140,   141,   145,   146,
     147,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "DOUBLE", "SYMBOL", "STRING",
  "CHAR", "CMP", "REL", "RETURN", "GLOBAL", "FUNCTION", "WHILE", "IF",
  "ELSE", "ELIF", "NIL", "TRUE", "FALSE", "DOT", "BREAK", "CONTINUE",
  "ERROR_LEXICAL_MARK", "'.'", "'='", "'+'", "'-'", "'*'", "'/'", "'^'",
  "'{'", "'}'", "','", "'('", "')'", "';'", "'['", "']'", "$accept",
  "start", "block", "stat_list", "param_list", "expr_list", "else_if_list",
  "stat", "expr", "assign", "func_def", "rel", "cmp", "add_sub", "mul_div",
  "factor", "func_call", "postfix", "term", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    46,    61,    43,    45,    42,    47,
      94,   123,   125,    44,    40,    41,    59,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    42,    43,    43,    43,
      44,    44,    44,    45,    45,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    47,    48,    48,
      48,    48,    49,    50,    50,    51,    51,    52,    52,    52,
      53,    53,    53,    54,    54,    55,    55,    55,    56,    56,
      56,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     2,     2,     1,     3,     1,     0,
       3,     1,     0,     6,     5,     2,     3,     3,     2,     6,
       5,     5,     7,     6,     8,     2,     2,     1,     3,     3,
       1,     1,     5,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     1,     4,     4,     1,     1,     4,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    51,    52,    53,    54,    58,     0,     0,     0,     0,
       0,    55,    56,    57,     0,     0,    62,     0,     0,    12,
       0,     2,     6,     0,    27,    30,    31,    34,    36,    39,
      42,    44,    47,    48,     0,    18,     0,     0,     0,     9,
       0,     0,    25,    26,    60,     0,     0,    11,     1,     5,
      15,     0,    12,     0,     0,     0,     0,     0,     0,     0,
      12,     0,     0,    17,    16,     9,     8,     0,     0,     0,
      59,     0,    61,    28,    29,     0,     0,    33,    35,    37,
      38,    40,    41,    43,     0,    50,     0,     0,     0,     0,
       0,     0,    10,    45,    46,    49,     0,     7,     0,    32,
      20,    21,    19,     4,     0,     0,     0,    23,     3,    22,
       0,     0,     0,     0,    24,     0,     0,     0,    14,     0,
      13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    20,    99,    21,    67,    46,   107,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -33
static const yytype_int16 yypact[] =
{
     107,   -33,   -33,   -33,   -33,   -33,   133,     0,    -2,    -4,
      -1,   -33,   -33,   -33,     1,     3,   -33,   184,   159,   159,
      44,   107,   -33,    12,    30,    42,    55,    74,    24,    29,
      57,    67,   -18,   -33,    68,   -33,    53,    69,    72,    86,
     159,   159,   -33,   -33,   -33,    73,    -9,   -33,   -33,   -33,
     -33,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,    98,   159,   -33,   -33,    86,   -33,   -29,    80,    81,
     -33,   159,   -33,    42,    55,     7,    42,    74,    24,    29,
      29,    57,    57,    67,    10,   -33,    84,    14,   102,    92,
      92,    92,   -33,   -33,   -33,   -33,    92,   -33,     4,   -33,
     -33,    37,   -33,   -33,    56,    92,    93,    65,   -33,   -33,
     159,    92,    97,   100,   -33,   159,    92,   108,   -33,    92,
     -33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -33,   -33,    89,    34,    77,   -32,   -33,   -20,    -6,   -33,
      41,    82,    94,    95,    16,    28,    87,   -33,   131
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      36,    49,    61,    38,    88,    37,    89,     1,     2,     3,
       4,     5,    45,    47,     6,     7,     8,     9,    10,    62,
      75,    11,    12,    13,    71,    14,    15,    16,    84,    72,
      40,    17,    39,    41,    68,    69,   103,    42,    18,    43,
      71,    19,    93,    71,    48,    94,    47,    88,    50,    96,
      55,    56,   105,   106,    47,    51,    86,    57,    58,     1,
       2,     3,     4,     5,    53,    92,     6,     7,     8,     9,
      10,    79,    80,    11,    12,    13,    52,    14,    15,    16,
     111,   112,    54,    17,    49,    81,    82,    59,   108,    63,
      18,    66,    73,    19,    76,    76,    76,    76,    76,    76,
      76,    60,    39,    85,   113,    64,    65,    97,    70,   117,
       1,     2,     3,     4,     5,    90,    91,     6,     7,     8,
       9,    10,    95,    98,    11,    12,    13,   110,    14,    15,
      16,   115,   104,    74,    17,   116,     1,     2,     3,     4,
       5,    18,    87,   119,    19,    34,    83,    77,    44,    78,
      11,    12,    13,     0,     0,     0,    16,     0,     0,     0,
      17,     0,     1,     2,     3,     4,     5,    18,     0,    35,
      19,    34,     0,     0,     0,     0,    11,    12,    13,   100,
     101,     0,    16,     0,     0,   102,    17,     1,     2,     3,
       4,     5,     0,    18,   109,     0,    19,     0,     0,     0,
     114,    11,    12,    13,     0,   118,     0,    16,   120,     0,
       0,    17,     0,     0,     0,     0,     0,     0,    18,     0,
       0,    19
};

static const yytype_int8 yycheck[] =
{
       6,    21,    20,     5,    33,     5,    35,     3,     4,     5,
       6,     7,    18,    19,    10,    11,    12,    13,    14,    37,
      52,    17,    18,    19,    33,    21,    22,    23,    60,    38,
      34,    27,    34,    34,    40,    41,    32,    36,    34,    36,
      33,    37,    35,    33,     0,    35,    52,    33,    36,    35,
      26,    27,    15,    16,    60,    25,    62,    28,    29,     3,
       4,     5,     6,     7,     9,    71,    10,    11,    12,    13,
      14,    55,    56,    17,    18,    19,    34,    21,    22,    23,
      15,    16,     8,    27,   104,    57,    58,    30,    32,    36,
      34,     5,    51,    37,    53,    54,    55,    56,    57,    58,
      59,    34,    34,     5,   110,    36,    34,     5,    35,   115,
       3,     4,     5,     6,     7,    35,    35,    10,    11,    12,
      13,    14,    38,    31,    17,    18,    19,    34,    21,    22,
      23,    34,    98,    51,    27,    35,     3,     4,     5,     6,
       7,    34,    65,    35,    37,    12,    59,    53,    17,    54,
      17,    18,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      27,    -1,     3,     4,     5,     6,     7,    34,    -1,    36,
      37,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    90,
      91,    -1,    23,    -1,    -1,    96,    27,     3,     4,     5,
       6,     7,    -1,    34,   105,    -1,    37,    -1,    -1,    -1,
     111,    17,    18,    19,    -1,   116,    -1,    23,   119,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    37
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    10,    11,    12,    13,
      14,    17,    18,    19,    21,    22,    23,    27,    34,    37,
      40,    42,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    12,    36,    47,     5,     5,    34,
      34,    34,    36,    36,    57,    47,    44,    47,     0,    46,
      36,    25,    34,     9,     8,    26,    27,    28,    29,    30,
      34,    20,    37,    36,    36,    34,     5,    43,    47,    47,
      35,    33,    38,    49,    50,    44,    49,    51,    52,    53,
      53,    54,    54,    55,    44,     5,    47,    43,    33,    35,
      35,    35,    47,    35,    35,    38,    35,     5,    31,    41,
      41,    41,    41,    32,    42,    15,    16,    45,    32,    41,
      34,    15,    16,    47,    41,    34,    35,    47,    41,    35,
      41
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (interpreter, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex (interpreter)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, interpreter); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct S_Interpreter* interpreter)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, interpreter)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    struct S_Interpreter* interpreter;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (interpreter);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct S_Interpreter* interpreter)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, interpreter)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    struct S_Interpreter* interpreter;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, interpreter);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, struct S_Interpreter* interpreter)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, interpreter)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    struct S_Interpreter* interpreter;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , interpreter);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, interpreter); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, struct S_Interpreter* interpreter)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, interpreter)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    struct S_Interpreter* interpreter;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (interpreter);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (struct S_Interpreter* interpreter);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (struct S_Interpreter* interpreter)
#else
int
yyparse (interpreter)
    struct S_Interpreter* interpreter;
#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 66 "simple.y"
    {interpreter->StatementList = (yyvsp[(1) - (1)].statement_list); (yyval.statement_list) = (yyvsp[(1) - (1)].statement_list);;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 69 "simple.y"
    {(yyval.code_block) = S_CreateCodeBlock(interpreter, (yyvsp[(2) - (3)].statement_list));;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 70 "simple.y"
    {(yyval.code_block) = S_CreateCodeBlock(interpreter, S_CreateStatementList(interpreter, 0));;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 73 "simple.y"
    {S_AddStatementToStatList(interpreter, (yyvsp[(1) - (2)].statement_list), (yyvsp[(2) - (2)].statement)); (yyval.statement_list) = (yyvsp[(1) - (2)].statement_list);;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 74 "simple.y"
    {(yyval.statement_list) = S_CreateStatementList(interpreter, (yyvsp[(1) - (1)].statement));;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 77 "simple.y"
    {S_AddParameterToParamList(interpreter, (yyvsp[(1) - (3)].parameter_list), S_CreateExpressionSymbol(interpreter, (yyvsp[(3) - (3)].id))); (yyval.parameter_list) = (yyvsp[(1) - (3)].parameter_list);;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 78 "simple.y"
    {(yyval.parameter_list) = S_CreateParamList(interpreter, S_CreateExpressionSymbol(interpreter, (yyvsp[(1) - (1)].id)));;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 79 "simple.y"
    {(yyval.parameter_list) = S_CreateParamList(interpreter, 0);;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 82 "simple.y"
    {S_AddExpressionToExpList(interpreter, (yyvsp[(1) - (3)].expression_list), (yyvsp[(3) - (3)].expression)); (yyval.expression_list) = (yyvsp[(1) - (3)].expression_list);;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 83 "simple.y"
    {(yyval.expression_list) = S_CreateExpList(interpreter, (yyvsp[(1) - (1)].expression));;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 84 "simple.y"
    {(yyval.expression_list) = S_CreateExpList(interpreter, 0);;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 87 "simple.y"
    {S_AddElifList(interpreter, (yyvsp[(1) - (6)].elif_list), (yyvsp[(4) - (6)].expression), (yyvsp[(6) - (6)].code_block)); (yyval.elif_list) = (yyvsp[(1) - (6)].elif_list);;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 88 "simple.y"
    {(yyval.elif_list) = S_CreateElifList(interpreter, (yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].code_block));;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 91 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementExpression(interpreter, (yyvsp[(1) - (2)].expression));;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 92 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementGlobal(interpreter, S_CreateExpressionSymbol(interpreter, (yyvsp[(2) - (3)].id)));;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 93 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementReturn(interpreter, (yyvsp[(2) - (3)].expression));;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 94 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementReturn(interpreter, (struct S_Expression*)S_CreateExpressionNil(interpreter));;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 95 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementFunctionDefine(interpreter, S_CreateExpressionSymbol(interpreter, (yyvsp[(2) - (6)].id)), (yyvsp[(4) - (6)].parameter_list), (yyvsp[(6) - (6)].code_block));;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 96 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementWhile(interpreter, (yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].code_block));;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 97 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementIf(interpreter, (yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].code_block), 0, 0);;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 98 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementIf(interpreter, (yyvsp[(3) - (7)].expression), (yyvsp[(5) - (7)].code_block), 0, (yyvsp[(7) - (7)].code_block));;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 99 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementIf(interpreter, (yyvsp[(3) - (6)].expression), (yyvsp[(5) - (6)].code_block), (yyvsp[(6) - (6)].elif_list), 0);;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 100 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementIf(interpreter, (yyvsp[(3) - (8)].expression), (yyvsp[(5) - (8)].code_block), (yyvsp[(6) - (8)].elif_list), (yyvsp[(8) - (8)].code_block));;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 101 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementBreak(interpreter);;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 102 "simple.y"
    {(yyval.statement) = (struct S_Statement*)S_CreateStatementContinue(interpreter);;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 105 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 108 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ASSIGN, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 109 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ASSIGN, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 110 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 111 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 114 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionFunctionDefine(interpreter, (yyvsp[(3) - (5)].parameter_list), (yyvsp[(5) - (5)].code_block));;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 117 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, (yyvsp[(2) - (3)].rel_type), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 118 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 121 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, (yyvsp[(2) - (3)].cmp_type), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 122 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 125 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_ADD, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 126 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_SUB, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 127 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 130 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_MUL, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 131 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_DIV, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 132 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 135 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionOp2(interpreter, OP2_FACTOR, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 136 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 139 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionFunctionCall(interpreter, (yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression_list));;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 140 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionFunctionCall(interpreter, (yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression_list));;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 141 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 145 "simple.y"
    {(yyval.expression) = (yyvsp[(1) - (1)].expression);;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 146 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionSubscript(interpreter, (yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression));;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 147 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionDot(interpreter, (yyvsp[(1) - (3)].expression), S_CreateExpressionSymbol(interpreter, (yyvsp[(3) - (3)].id)));;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 150 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionInteger(interpreter, (yyvsp[(1) - (1)].int_number));;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 151 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionDouble(interpreter, (yyvsp[(1) - (1)].double_number));;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 152 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionSymbol(interpreter, (yyvsp[(1) - (1)].id));;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 153 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionString(interpreter, (yyvsp[(1) - (1)].string));;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 154 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionNil(interpreter);;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 155 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionTrue(interpreter);;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 156 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionFalse(interpreter);;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 157 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionChar(interpreter, (yyvsp[(1) - (1)].char_ascii));;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 158 "simple.y"
    {(yyval.expression) = (yyvsp[(2) - (3)].expression);;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 159 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionNegation(interpreter, (yyvsp[(2) - (2)].expression));;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 160 "simple.y"
    {(yyval.expression) = (struct S_Expression*)S_CreateExpressionArray(interpreter, (yyvsp[(2) - (3)].expression_list));;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 161 "simple.y"
    {YYABORT;;}
    break;



/* Line 1455 of yacc.c  */
#line 1980 "simple.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (interpreter, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (interpreter, yymsg);
	  }
	else
	  {
	    yyerror (interpreter, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, interpreter);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, interpreter);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (interpreter, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, interpreter);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, interpreter);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 163 "simple.y"




