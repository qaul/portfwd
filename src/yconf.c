/* A Bison parser, made by GNU Bison 1.875a.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_NAME = 258,
     TK_TCP = 259,
     TK_UDP = 260,
     TK_COLON = 261,
     TK_SCOLON = 262,
     TK_COMMA = 263,
     TK_SLASH = 264,
     TK_RANGE = 265,
     TK_LBRACE = 266,
     TK_RBRACE = 267,
     TK_ARROW = 268,
     TK_ACTV = 269,
     TK_PASV = 270,
     TK_USER = 271,
     TK_GROUP = 272,
     TK_BIND = 273,
     TK_LISTEN = 274,
     TK_SOURCE = 275,
     TK_STRING = 276,
     TK_FRAGILE = 277,
     TK_ILLEGAL = 278
   };
#endif
#define TK_NAME 258
#define TK_TCP 259
#define TK_UDP 260
#define TK_COLON 261
#define TK_SCOLON 262
#define TK_COMMA 263
#define TK_SLASH 264
#define TK_RANGE 265
#define TK_LBRACE 266
#define TK_RBRACE 267
#define TK_ARROW 268
#define TK_ACTV 269
#define TK_PASV 270
#define TK_USER 271
#define TK_GROUP 272
#define TK_BIND 273
#define TK_LISTEN 274
#define TK_SOURCE 275
#define TK_STRING 276
#define TK_FRAGILE 277
#define TK_ILLEGAL 278




/* Copy the first part of user declarations.  */
#line 7 "conf.y"


#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "util.h"
#include "solve.h"
#include "port_pair.h"
#include "proto_map.hpp"
#include "entry.hpp"
#include "dst_addr.hpp"
#include "director.hpp"
#include "portfwd.h"

/*
 * These are from the lexical analyzer defined in conf.lex
 */
extern int yylex();
extern char yytext[];
extern void show_last_token();
extern int conf_line_number;
extern char conf_ident[];
extern char conf_lex_str_buf[];

/*
 * Some useful constants
 */
const char *const ANY_ADDR = "0.0.0.0";
const int MIN_MASK_LEN = 0;
const int MAX_MASK_LEN = 32;

/*
 * We store the number of syntax errors here
 */
int conf_syntax_errors = 0;

void yyerror(const char *msg)
{
  ++conf_syntax_errors;
  syslog(LOG_ERR, "Syntax error: %s\n", msg);
  show_last_token();
}

/* Variaveis Auxiliares */

proto_t curr_proto;
vector<from_addr*> *from_vector;
vector<to_addr*>   *dst_vector;
vector<host_map*>  *host_vector;
vector<int>        *port_vector;
vector<proto_map*> *map_vector;
vector<entry*>     *entry_vector = new vector<entry*>();

int            conf_user   = -1;
int            conf_group  = -1;

const struct ip_addr conf_any_addr = solve_hostname(ANY_ADDR);
struct ip_addr conf_listen         = conf_any_addr;
struct ip_addr conf_source         = conf_any_addr;
struct ip_addr *conf_src           = 0;

/* Funcoes Auxiliares */

int mask_len_value(const char *len)
{
  return atoi(len);
}

void set_protoname(proto_t proto)
{
  curr_proto = proto;
  ONVERBOSE(syslog(LOG_DEBUG, "Protocol: %s\n", get_protoname(curr_proto)))
}

int use_port(char *portname)
{
  int port = solve_port(portname, get_protoname(curr_proto));
  free(portname);
  return port;
}

struct ip_addr use_hostname(char *hostname)
{
  struct ip_addr ip = solve_hostname(hostname);
  free(hostname);
  return ip;
}

net_portion *use_hostprefix(char *hostname, int prefix_len)
{
  return new net_portion(use_hostname(hostname), prefix_len);  
}

to_addr *use_dstaddr(char *hostname, int port)
{
  return new dst_addr(on_the_fly_dns ? safe_strdup(hostname) : 0, 
		     use_hostname(hostname), 
		     port);
}



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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 135 "conf.y"
typedef union YYSTYPE {
	int	           int_type;
  	bool               bool_type;
        char               *str_type;
        port_pair          *port_type;
        net_portion        *net_type;
	from_addr   	   *from_type;
	vector<from_addr*> *from_list_type;
	to_addr   	   *dst_type;
	vector<to_addr*>   *dst_list_type;
	host_map           *host_map_type;
	vector<host_map*>  *host_list_type;
	vector<int>        *port_list_type;
	proto_map          *map_type;
	vector<proto_map*> *map_list_type;
	entry		   *entry_type;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 243 "yconf.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 170 "conf.y"

  /* Simbolo nao-terminal inicial */


/* Line 214 of yacc.c.  */
#line 258 "yconf.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   77

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  23
/* YYNRULES -- Number of rules. */
#define YYNRULES  49
/* YYNRULES -- Number of states. */
#define YYNSTATES  90

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
static const unsigned char yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    13,    15,    18,
      21,    24,    27,    30,    35,    39,    40,    42,    43,    44,
      48,    50,    54,    59,    66,    73,    82,    91,    93,    95,
      99,   101,   105,   109,   111,   115,   119,   121,   123,   127,
     128,   130,   133,   137,   140,   141,   144,   146,   149,   152
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      25,     0,    -1,    -1,    26,    -1,    27,    -1,    26,    27,
      -1,    29,    -1,    28,    -1,    16,     3,    -1,    17,     3,
      -1,    19,     3,    -1,    20,     3,    -1,    18,     3,    -1,
      30,     4,    31,    33,    -1,     5,    32,    33,    -1,    -1,
      22,    -1,    -1,    -1,    11,    34,    12,    -1,    35,    -1,
      34,     7,    35,    -1,    37,    11,    38,    12,    -1,    37,
      14,    36,    11,    38,    12,    -1,    37,    15,    36,    11,
      38,    12,    -1,    37,    14,    36,    15,    36,    11,    38,
      12,    -1,    37,    15,    36,    14,    36,    11,    38,    12,
      -1,     3,    -1,    36,    -1,    37,     8,    36,    -1,    39,
      -1,    38,     7,    39,    -1,    42,    13,    40,    -1,    41,
      -1,    40,     8,    41,    -1,    36,     6,    36,    -1,    21,
      -1,    43,    -1,    42,     8,    43,    -1,    -1,    44,    -1,
       6,    46,    -1,    44,     6,    46,    -1,    36,    45,    -1,
      -1,     9,     3,    -1,    36,    -1,    36,    10,    -1,    10,
      36,    -1,    36,    10,    36,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   178,   178,   179,   181,   182,   184,   185,   187,   188,
     189,   190,   194,   196,   197,   199,   200,   202,   203,   205,
     207,   212,   217,   220,   224,   228,   233,   239,   241,   246,
     251,   256,   261,   265,   270,   275,   279,   283,   288,   293,
     296,   299,   302,   306,   311,   312,   314,   318,   322,   326
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_NAME", "TK_TCP", "TK_UDP", "TK_COLON", 
  "TK_SCOLON", "TK_COMMA", "TK_SLASH", "TK_RANGE", "TK_LBRACE", 
  "TK_RBRACE", "TK_ARROW", "TK_ACTV", "TK_PASV", "TK_USER", "TK_GROUP", 
  "TK_BIND", "TK_LISTEN", "TK_SOURCE", "TK_STRING", "TK_FRAGILE", 
  "TK_ILLEGAL", "$accept", "conf", "stmt_list", "stmt", "global_option", 
  "entry", "fragile", "set_proto_tcp", "set_proto_udp", "section", 
  "map_list", "map", "name", "port_list", "host_list", "host_map", 
  "dst_list", "dst", "from_list", "from", "host_prefix", "prefix_length", 
  "port_range", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    24,    25,    25,    26,    26,    27,    27,    28,    28,
      28,    28,    28,    29,    29,    30,    30,    31,    32,    33,
      34,    34,    35,    35,    35,    35,    35,    36,    37,    37,
      38,    38,    39,    40,    40,    41,    41,    42,    42,    43,
      43,    43,    43,    44,    45,    45,    46,    46,    46,    46
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     1,     2,     2,
       2,     2,     2,     4,     3,     0,     1,     0,     0,     3,
       1,     3,     4,     6,     6,     8,     8,     1,     1,     3,
       1,     3,     3,     1,     3,     3,     1,     1,     3,     0,
       1,     2,     3,     2,     0,     2,     1,     2,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,    18,     0,     0,     0,     0,     0,    16,     0,     3,
       4,     7,     6,     0,     0,     8,     9,    12,    10,    11,
       1,     5,    17,     0,    14,     0,    27,     0,    20,    28,
       0,    13,     0,    19,     0,    39,     0,     0,    21,    29,
       0,    44,     0,    30,     0,    37,    40,     0,     0,     0,
      46,    41,     0,    43,    39,    22,    39,     0,     0,    39,
       0,    39,     0,    48,    47,    45,    31,    38,    36,     0,
      32,    33,    42,     0,     0,     0,     0,    49,     0,     0,
      23,    39,    24,    39,    35,    34,     0,     0,    25,    26
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     8,     9,    10,    11,    12,    13,    25,    14,    24,
      27,    28,    41,    30,    42,    43,    70,    71,    44,    45,
      46,    53,    51
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -59
static const yysigned_char yypact[] =
{
       2,   -59,    26,    27,    39,    55,    56,   -59,     4,     2,
     -59,   -59,   -59,    57,    49,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,    59,   -59,    49,   -59,    20,   -59,   -59,
      32,   -59,    59,   -59,    59,    -1,    59,    59,   -59,   -59,
       5,    54,    24,   -59,    25,   -59,    58,     1,    40,    59,
      60,   -59,    62,   -59,    -1,   -59,    -1,     7,     5,    -1,
      59,    -1,    59,   -59,    59,   -59,   -59,   -59,   -59,    61,
      63,   -59,   -59,    37,    64,    38,    65,   -59,    59,     7,
     -59,    -1,   -59,    -1,   -59,   -59,    41,    45,   -59,   -59
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -59,   -59,   -59,    68,   -59,   -59,   -59,   -59,   -59,    43,
     -59,    34,   -23,   -59,   -58,    15,   -59,    -7,   -59,    17,
     -59,   -59,    16
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -16
static const yysigned_char yytable[] =
{
      29,    73,    26,    75,    20,    40,   -15,     1,    26,    29,
      26,    39,    59,    47,    48,    49,    60,    50,     2,     3,
       4,     5,     6,    86,     7,    87,    63,    32,    68,    15,
      16,    54,    33,    56,    69,    50,    55,    74,    57,    76,
      34,    77,    17,    35,    54,    54,    36,    37,    54,    80,
      82,    61,    54,    88,    62,    84,    69,    89,    18,    19,
      23,    22,    26,    52,    58,    65,    38,    78,    31,    66,
      64,    79,    85,    67,    72,    81,    83,    21
};

static const unsigned char yycheck[] =
{
      23,    59,     3,    61,     0,     6,     4,     5,     3,    32,
       3,    34,    11,    36,    37,    10,    15,    40,    16,    17,
      18,    19,    20,    81,    22,    83,    49,     7,    21,     3,
       3,     7,    12,     8,    57,    58,    12,    60,    13,    62,
       8,    64,     3,    11,     7,     7,    14,    15,     7,    12,
      12,    11,     7,    12,    14,    78,    79,    12,     3,     3,
      11,     4,     3,     9,     6,     3,    32,     6,    25,    54,
      10,     8,    79,    56,    58,    11,    11,     9
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     5,    16,    17,    18,    19,    20,    22,    25,    26,
      27,    28,    29,    30,    32,     3,     3,     3,     3,     3,
       0,    27,     4,    11,    33,    31,     3,    34,    35,    36,
      37,    33,     7,    12,     8,    11,    14,    15,    35,    36,
       6,    36,    38,    39,    42,    43,    44,    36,    36,    10,
      36,    46,     9,    45,     7,    12,     8,    13,     6,    11,
      15,    11,    14,    36,    10,     3,    39,    43,    21,    36,
      40,    41,    46,    38,    36,    38,    36,    36,     6,     8,
      12,    11,    12,    11,    36,    41,    38,    38,    12,    12
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
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
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:
#line 184 "conf.y"
    { entry_vector-> push(yyvsp[0].entry_type); }
    break;

  case 8:
#line 187 "conf.y"
    { conf_user = solve_user(conf_ident); }
    break;

  case 9:
#line 188 "conf.y"
    { conf_group = solve_group(conf_ident); }
    break;

  case 10:
#line 189 "conf.y"
    { conf_listen = solve_hostname(conf_ident); }
    break;

  case 11:
#line 190 "conf.y"
    { 
					conf_source = solve_hostname(conf_ident); 
					conf_src = &conf_source;
		}
    break;

  case 12:
#line 194 "conf.y"
    { conf_listen = solve_hostname(conf_ident); }
    break;

  case 13:
#line 196 "conf.y"
    { yyval.entry_type = new entry(P_TCP, yyvsp[0].map_list_type, yyvsp[-3].bool_type); }
    break;

  case 14:
#line 197 "conf.y"
    { yyval.entry_type = new entry(P_UDP, yyvsp[0].map_list_type, 0 /* false */); }
    break;

  case 15:
#line 199 "conf.y"
    { yyval.bool_type = 0; /* false */ }
    break;

  case 16:
#line 200 "conf.y"
    { yyval.bool_type = 1; /* true */ }
    break;

  case 17:
#line 202 "conf.y"
    { set_protoname(P_TCP); }
    break;

  case 18:
#line 203 "conf.y"
    { set_protoname(P_UDP); }
    break;

  case 19:
#line 205 "conf.y"
    { yyval.map_list_type = yyvsp[-1].map_list_type; }
    break;

  case 20:
#line 207 "conf.y"
    {
			map_vector = new vector<proto_map*>();
			map_vector->push(yyvsp[0].map_type);
			yyval.map_list_type = map_vector;
		}
    break;

  case 21:
#line 212 "conf.y"
    {
			map_vector->push(yyvsp[0].map_type);
			yyval.map_list_type = map_vector;
		}
    break;

  case 22:
#line 217 "conf.y"
    {
			yyval.map_type = new proto_map(yyvsp[-3].port_list_type, yyvsp[-1].host_list_type, 0, 0, conf_user, conf_group, conf_listen, conf_src);
		}
    break;

  case 23:
#line 220 "conf.y"
    {
		        struct ip_addr ip = use_hostname(yyvsp[-3].str_type);
			yyval.map_type = new proto_map(yyvsp[-5].port_list_type, yyvsp[-1].host_list_type, &ip, 0, conf_user, conf_group, conf_listen, conf_src);
		}
    break;

  case 24:
#line 224 "conf.y"
    {
		        struct ip_addr ip = use_hostname(yyvsp[-3].str_type);
			yyval.map_type = new proto_map(yyvsp[-5].port_list_type, yyvsp[-1].host_list_type, 0, &ip, conf_user, conf_group, conf_listen, conf_src);
		}
    break;

  case 25:
#line 228 "conf.y"
    {
		        struct ip_addr ip1 = use_hostname(yyvsp[-5].str_type);
		        struct ip_addr ip2 = use_hostname(yyvsp[-3].str_type);
			yyval.map_type = new proto_map(yyvsp[-7].port_list_type, yyvsp[-1].host_list_type, &ip1, &ip2, conf_user, conf_group, conf_listen, conf_src);
		}
    break;

  case 26:
#line 233 "conf.y"
    {
		        struct ip_addr ip1 = use_hostname(yyvsp[-5].str_type);
		        struct ip_addr ip2 = use_hostname(yyvsp[-3].str_type);
			yyval.map_type = new proto_map(yyvsp[-7].port_list_type, yyvsp[-1].host_list_type, &ip2, &ip1, conf_user, conf_group, conf_listen, conf_src);
		}
    break;

  case 27:
#line 239 "conf.y"
    { yyval.str_type = safe_strdup(conf_ident); }
    break;

  case 28:
#line 241 "conf.y"
    {
			port_vector = new vector<int>();
			port_vector->push(use_port(yyvsp[0].str_type));
			yyval.port_list_type = port_vector; 
		}
    break;

  case 29:
#line 246 "conf.y"
    {
			port_vector->push(use_port(yyvsp[0].str_type));
			yyval.port_list_type = port_vector; 
		}
    break;

  case 30:
#line 251 "conf.y"
    {
              		host_vector = new vector<host_map*>();
			host_vector->push(yyvsp[0].host_map_type);
			yyval.host_list_type = host_vector;
		}
    break;

  case 31:
#line 256 "conf.y"
    {
			host_vector->push(yyvsp[0].host_map_type);
			yyval.host_list_type = host_vector;
		}
    break;

  case 32:
#line 261 "conf.y"
    {
			yyval.host_map_type = new host_map(yyvsp[-2].from_list_type, yyvsp[0].dst_list_type);
		}
    break;

  case 33:
#line 265 "conf.y"
    {
			dst_vector = new vector<to_addr*>();
			dst_vector->push(yyvsp[0].dst_type);
			yyval.dst_list_type = dst_vector;
                }
    break;

  case 34:
#line 270 "conf.y"
    {
			dst_vector->push(yyvsp[0].dst_type);
			yyval.dst_list_type = dst_vector;
                }
    break;

  case 35:
#line 275 "conf.y"
    {
			int port = use_port(yyvsp[0].str_type);   /* solve portname */
			yyval.dst_type = use_dstaddr(yyvsp[-2].str_type, port); /* new dst_addr() */
                }
    break;

  case 36:
#line 279 "conf.y"
    {
                        yyval.dst_type = new director(conf_lex_str_buf);
		}
    break;

  case 37:
#line 283 "conf.y"
    {
			from_vector = new vector<from_addr*>();
			from_vector->push(yyvsp[0].from_type);
			yyval.from_list_type = from_vector;
		}
    break;

  case 38:
#line 288 "conf.y"
    {
			from_vector->push(yyvsp[0].from_type);
			yyval.from_list_type = from_vector;
		}
    break;

  case 39:
#line 293 "conf.y"
    { 
			yyval.from_type = new from_addr(new net_portion(solve_hostname(ANY_ADDR), MIN_MASK_LEN), new port_pair(FIRST_PORT, LAST_PORT)); 
		}
    break;

  case 40:
#line 296 "conf.y"
    { 
			yyval.from_type = new from_addr(yyvsp[0].net_type, new port_pair(FIRST_PORT, LAST_PORT)); 
		}
    break;

  case 41:
#line 299 "conf.y"
    { 
			yyval.from_type = new from_addr(new net_portion(solve_hostname(ANY_ADDR), MIN_MASK_LEN), yyvsp[0].port_type); 
		}
    break;

  case 42:
#line 302 "conf.y"
    { 
			yyval.from_type = new from_addr(yyvsp[-2].net_type, yyvsp[0].port_type); 
		}
    break;

  case 43:
#line 306 "conf.y"
    { 
			/* use_hostprefix(): new net_portion() */
  			yyval.net_type = use_hostprefix(yyvsp[-1].str_type, yyvsp[0].int_type); 
		}
    break;

  case 44:
#line 311 "conf.y"
    { yyval.int_type = MAX_MASK_LEN; }
    break;

  case 45:
#line 312 "conf.y"
    { yyval.int_type = mask_len_value(conf_ident); }
    break;

  case 46:
#line 314 "conf.y"
    {
			int port = use_port(yyvsp[0].str_type);
			yyval.port_type = new port_pair(port, port);
		}
    break;

  case 47:
#line 318 "conf.y"
    { 
			yyval.port_type = new port_pair(use_port(yyvsp[-1].str_type), 
					   LAST_PORT); 
		}
    break;

  case 48:
#line 322 "conf.y"
    { 
			yyval.port_type = new port_pair(FIRST_PORT, 
				           use_port(yyvsp[0].str_type)); 
		}
    break;

  case 49:
#line 326 "conf.y"
    { 
			yyval.port_type = new port_pair(use_port(yyvsp[-2].str_type),
				           use_port(yyvsp[0].str_type));
		}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1502 "yconf.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 332 "conf.y"


/* C code */

/* eof: conf.y */


