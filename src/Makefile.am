CFLAGS   = -Wall -ggdb -O3 -DPORTFWD_CONF=\"$(sysconfdir)/portfwd.cfg\"
CXXFLAGS = $(CFLAGS)
INCLUDES = -I$(top_srcdir)/src

sbin_PROGRAMS = portfwd
portfwd_SOURCES = addr.cc from_addr.cc host_map.cc portfwd.cc \
	dst_addr.cc director.cc entry.cc net_portion.cc \
	proto_map.cc forward.cc  port_pair.cc  solve.cc \
	yconf.c lexconf.c util.cc 

LDADD = getopt.o getopt1.o

yconf.c: conf.y
	$(YACC) -v -d -o $@ $<

lexconf.c: conf.lex
	$(LEX) -t $< > $@
