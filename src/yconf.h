typedef union {
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
} YYSTYPE;
#define	TK_NAME	257
#define	TK_TCP	258
#define	TK_UDP	259
#define	TK_COLON	260
#define	TK_SCOLON	261
#define	TK_COMMA	262
#define	TK_SLASH	263
#define	TK_RANGE	264
#define	TK_LBRACE	265
#define	TK_RBRACE	266
#define	TK_ARROW	267
#define	TK_ACTV	268
#define	TK_PASV	269
#define	TK_USER	270
#define	TK_GROUP	271
#define	TK_BIND	272
#define	TK_ILLEGAL	273


extern YYSTYPE yylval;
