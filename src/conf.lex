/*
  conf.lex

  $Id: conf.lex,v 1.1 2001/05/15 00:25:01 evertonm Exp $
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

const int IDENT_BUF_SIZE = 64;

int conf_line_number = 1;
int comment_nesting = 0;

char conf_ident[IDENT_BUF_SIZE];

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

NAME		[a-zA-Z0-9][a-zA-Z0-9_.-]*

%%

%{
		/* Reserved Symbols */
%}

tcp		return trap_tk(TK_TCP);
udp		return trap_tk(TK_UDP);

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
bind-address        return trap_tk(TK_BIND);

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
