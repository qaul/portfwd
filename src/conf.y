/*
  conf.y

  $Id: conf.y,v 1.3 2001/11/19 19:34:44 evertonm Exp $
*/

%{

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "util.h"
#include "solve.h"
#include "port_pair.h"
#include "proto_map.hpp"
#include "entry.hpp"
#include "portfwd.h"

/*
 * These are from the lexical analyzer defined in conf.lex
 */
extern int yylex();
extern char yytext[];
extern void show_last_token();
extern int conf_line_number;
extern char conf_ident[];

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

to_addr *use_toaddr(char *hostname, int port)
{
  return new to_addr(on_the_fly_dns ? safe_strdup(hostname) : 0, 
		     use_hostname(hostname), 
		     port);
}

%}

%token TK_NAME

%token TK_TCP
%token TK_UDP

%token TK_COLON
%token TK_SCOLON
%token TK_COMMA
%token TK_SLASH
%token TK_RANGE
%token TK_LBRACE
%token TK_RBRACE
%token TK_ARROW
%token TK_ACTV
%token TK_PASV
%token TK_USER
%token TK_GROUP
%token TK_BIND
%token TK_LISTEN
%token TK_SOURCE

%token TK_ILLEGAL

%union {
	int	           int_type;
        char               *str_type;
        port_pair          *port_type;
        net_portion        *net_type;
	from_addr   	   *from_type;
	vector<from_addr*> *from_list_type;
	host_map           *host_map_type;
	vector<host_map*>  *host_list_type;
	vector<int>        *port_list_type;
	proto_map          *map_type;
	vector<proto_map*> *map_list_type;
	entry		   *entry_type;
}

%type <int_type>	prefix_length;
%type <port_type>       port_range;
%type <str_type>        name;
%type <net_type>        host_prefix;
%type <from_type>       from;
%type <from_list_type>  from_list;
%type <host_map_type>   host_map;
%type <host_list_type>  host_list;
%type <port_list_type>  port_list;
%type <map_type>        map;
%type <map_list_type>   map_list;
%type <map_list_type>   section;
%type <entry_type>      entry;

%{
  /* Simbolo nao-terminal inicial */
%}

%start conf

%%

conf:		/* empty */ |
                stmt_list ;

stmt_list:      stmt |
                stmt_list stmt ;

stmt:           entry { entry_vector-> push($1); } |
                global_option ;

global_option:  TK_USER TK_NAME { conf_user = solve_user(conf_ident); } |
                TK_GROUP TK_NAME { conf_group = solve_group(conf_ident); } |
                TK_LISTEN TK_NAME { conf_listen = solve_hostname(conf_ident); } |
                TK_SOURCE TK_NAME { 
					conf_source = solve_hostname(conf_ident); 
					conf_src = &conf_source;
		} |
		TK_BIND TK_NAME { conf_listen = solve_hostname(conf_ident); };

entry:		TK_TCP set_proto_tcp section { $$ = new entry(P_TCP, $3); } | 
         	TK_UDP set_proto_udp section { $$ = new entry(P_UDP, $3); };

set_proto_tcp:	{ set_protoname(P_TCP); } ;
set_proto_udp:  { set_protoname(P_UDP); } ;

section:        TK_LBRACE map_list TK_RBRACE { $$ = $2; } ;

map_list:	map {
			map_vector = new vector<proto_map*>();
			map_vector->push($1);
			$$ = map_vector;
		} | 
		map_list TK_SCOLON map {
			map_vector->push($3);
			$$ = map_vector;
		} ;

map:	        port_list TK_LBRACE host_list TK_RBRACE {
			$$ = new proto_map($1, $3, 0, 0, conf_user, conf_group, conf_listen, conf_src);
		} | 
                port_list TK_ACTV name TK_LBRACE host_list TK_RBRACE {
		        struct ip_addr ip = use_hostname($3);
			$$ = new proto_map($1, $5, &ip, 0, conf_user, conf_group, conf_listen, conf_src);
		} |
                port_list TK_PASV name TK_LBRACE host_list TK_RBRACE {
		        struct ip_addr ip = use_hostname($3);
			$$ = new proto_map($1, $5, 0, &ip, conf_user, conf_group, conf_listen, conf_src);
		} |
                port_list TK_ACTV name TK_PASV name TK_LBRACE host_list TK_RBRACE {
		        struct ip_addr ip1 = use_hostname($3);
		        struct ip_addr ip2 = use_hostname($5);
			$$ = new proto_map($1, $7, &ip1, &ip2, conf_user, conf_group, conf_listen, conf_src);
		} |
                port_list TK_PASV name TK_ACTV name TK_LBRACE host_list TK_RBRACE {
		        struct ip_addr ip1 = use_hostname($3);
		        struct ip_addr ip2 = use_hostname($5);
			$$ = new proto_map($1, $7, &ip2, &ip1, conf_user, conf_group, conf_listen, conf_src);
		} ;

name:           TK_NAME { $$ = safe_strdup(conf_ident); } ;

port_list:      name {
			port_vector = new vector<int>();
			port_vector->push(use_port($1));
			$$ = port_vector; 
		} |
                port_list TK_COMMA name {
			port_vector->push(use_port($3));
			$$ = port_vector; 
		} ;

host_list:	host_map {
              		host_vector = new vector<host_map*>();
			host_vector->push($1);
			$$ = host_vector;
		} |
	        host_list TK_SCOLON host_map {
			host_vector->push($3);
			$$ = host_vector;
		} ;

host_map:	from_list TK_ARROW name TK_COLON name {
			int port = use_port($5);
			to_addr *to = use_toaddr($3, port); /* new to_addr() */
			$$ = new host_map($1, to);
		} ;

from_list:	from {
			from_vector = new vector<from_addr*>();
			from_vector->push($1);
			$$ = from_vector;
		} |
		from_list TK_COMMA from {
			from_vector->push($3);
			$$ = from_vector;
		} ;

from: 	        /* empty */ { 
			$$ = new from_addr(new net_portion(solve_hostname(ANY_ADDR), MIN_MASK_LEN), new port_pair(FIRST_PORT, LAST_PORT)); 
		} |
                host_prefix { 
			$$ = new from_addr($1, new port_pair(FIRST_PORT, LAST_PORT)); 
		} |
                TK_COLON port_range { 
			$$ = new from_addr(new net_portion(solve_hostname(ANY_ADDR), MIN_MASK_LEN), $2); 
		} |
                host_prefix TK_COLON port_range { 
			$$ = new from_addr($1, $3); 
		} ;

host_prefix:    name prefix_length { 
			/* use_hostprefix(): new net_portion() */
  			$$ = use_hostprefix($1, $2); 
		} ;

prefix_length:  /* empty */ { $$ = MAX_MASK_LEN; } |
                TK_SLASH TK_NAME { $$ = mask_len_value(conf_ident); } ;

port_range:	name {
			int port = use_port($1);
			$$ = new port_pair(port, port);
		} |
	      	name TK_RANGE { 
			$$ = new port_pair(use_port($1), 
					   LAST_PORT); 
		} |
	      	TK_RANGE name { 
			$$ = new port_pair(FIRST_PORT, 
				           use_port($2)); 
		} |
	      	name TK_RANGE name { 
			$$ = new port_pair(use_port($1),
				           use_port($3));
		} ;


%%

/* C code */

/* eof: conf.y */

