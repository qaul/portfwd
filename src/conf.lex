/*
  conf.lex

  $Id: conf.lex,v 1.4 2004/01/28 19:14:10 evertonm Exp $
 */

%{

#include <stdio.h>
#include <syslog.h>

  /* Incluir antes de "yconf.h" */
#include "port_pair.h"
#include "net_portion.h"
#include "from_addr.hpp"
#include "host_map.hpp"
#include "proto_map.hpp"
#include "entry.hpp"
#include "vector.hpp"

#include "yconf.h"
#include "util.h"

int conf_line_number = 1;
int comment_nesting = 0;

const int IDENT_BUF_SIZE = 64;
char conf_ident[IDENT_BUF_SIZE];

const int CONF_LEX_STR_BUF_SIZE = 1024;
char conf_lex_str_buf[CONF_LEX_STR_BUF_SIZE];
char *conf_lex_str_end = conf_lex_str_buf + CONF_LEX_STR_BUF_SIZE;
char *conf_lex_str_curr;

static int add_str_char(char c)
{
  if (conf_lex_str_curr < conf_lex_str_buf)
    return 1; /* underflow */

  if (conf_lex_str_curr >= conf_lex_str_end)
    return 2; /* overflow */

  *conf_lex_str_curr = c;

  ++conf_lex_str_curr;

  return 0;
}

#define STR_CHAR(c)                       \
{                                         \
  int result = add_str_char(c);           \
  if (result) {                           \
    switch (result) {                     \
    case 1:                               \
      conf_error("string underflow");     \
      break;                              \
    case 2:                               \
      conf_error("string overflow");      \
      break;                              \
    default:                              \
      conf_error("unknown string error"); \
    }                                     \
    return trap_tk(TK_ILLEGAL);           \
  }                                       \
}

int trap_tk(int tk)
{
  ONVERBOSE(syslog(LOG_DEBUG, "line %4d, token %4d '%s'\n", conf_line_number, tk, yytext));
  return tk;
}

void show_last_token() 
{
  syslog(LOG_ERR, "Last token: '%s' at line: %d\n", yytext, conf_line_number);
}

void conf_error(const char *msg)
{
  syslog(LOG_ERR, "Lexical error: %s\n", msg);
  show_last_token();
}

%}

%array
%option noyywrap
%x Comment
%x String

NAME		[a-zA-Z0-9][a-zA-Z0-9_.-]*

%%

%{
		/* Reserved Symbols */
%}

tcp		return trap_tk(TK_TCP);
udp		return trap_tk(TK_UDP);
fragile        return trap_tk(TK_FRAGILE);

":"		return trap_tk(TK_COLON);
";"   		return trap_tk(TK_SCOLON);
","		return trap_tk(TK_COMMA);
"/"		return trap_tk(TK_SLASH);
"+"		return trap_tk(TK_RANGE);
"{"		return trap_tk(TK_LBRACE);
"}"		return trap_tk(TK_RBRACE);
"=>"		return trap_tk(TK_ARROW);

ftp-active-mode-on  return trap_tk(TK_ACTV);
ftp-passive-mode-on return trap_tk(TK_PASV);
user                return trap_tk(TK_USER);
group               return trap_tk(TK_GROUP);
bind-address        { 
                    syslog(LOG_ERR, "Found deprecated 'bind-address' token at line %d, please use 'listen-on' instead", conf_line_number);
                    return trap_tk(TK_BIND);
                    }
listen-on           return trap_tk(TK_LISTEN);
source-address      return trap_tk(TK_SOURCE);

%{
		/* Names and Numbers */
%}

{NAME}		{
		safe_strcpy(conf_ident, yytext, IDENT_BUF_SIZE);
  		return trap_tk(TK_NAME);
		}

%{
		/* Blanks */
%}

[ \r\t]   	/* Ignore */
\n		++conf_line_number;

%{
                /* Strings */
%}

"["             {
                BEGIN(String);
		conf_lex_str_curr = conf_lex_str_buf;
                }
<String>\n      {
                ++conf_line_number;
		STR_CHAR(*yytext);
                }
<String>[^\]]   STR_CHAR(*yytext);
<String>"]"     {
                BEGIN(INITIAL);
		STR_CHAR('\0');
		return trap_tk(TK_STRING);
                }
<String><<EOF>> {
		conf_error("EOF at unterminated string");
		return trap_tk(TK_ILLEGAL);
                }

%{
		/* Comments */
%}

"/*"                    ++comment_nesting; BEGIN(Comment);
<Comment>"/*"           ++comment_nesting; 
<Comment>"*/"           if (!--comment_nesting) BEGIN(INITIAL);
<Comment>\n             ++conf_line_number;
<Comment>.              /* Ignore */
<Comment><<EOF>>        {
			conf_error("EOF at unterminated comment");
			return trap_tk(TK_ILLEGAL);
			}

%{
		/* Unknown */
%}

.		{ 
		conf_error("Unknown token");
		return trap_tk(TK_ILLEGAL);
		}

%%

/* C code */

/* eof: conf.lex */
