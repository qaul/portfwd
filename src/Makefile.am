CXXFLAGS = -Wall -ggdb -O3
CFLAGS   = -Wall -ggdb -O3
INCLUDES = -I$(top_srcdir)/src

sbin_PROGRAMS = portfwd
portfwd_SOURCES = addr.cc from_addr.cc host_map.cc portfwd.cc to_addr.cc \
	entry.cc      net_portion.cc  proto_map.cc  \
	forward.cc  port_pair.cc  solve.cc yconf.c lexconf.c util.cc 

LDADD = getopt.o getopt1.o
yconf.c: conf.y
	$(YACC) -v -d -o $@ $<

lexconf.c: conf.lex
	$(LEX) -t $< > $@
