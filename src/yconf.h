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
/* Line 1240 of yacc.c.  */
#line 101 "yconf.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



