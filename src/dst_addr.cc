/*
  dst_addr.cc

  $Id: dst_addr.cc,v 1.3 2002/05/07 03:15:36 evertonm Exp $
 */

#include <syslog.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "addr.h"
#include "dst_addr.hpp"
#include "solve.h"

void dst_addr::show() const
{
  show_addr(&address);
  syslog(LOG_INFO, ":%d", port);
}

int dst_addr::get_addr(const char *protoname, 
	const struct sockaddr_in *cli_sa, 
	const struct sockaddr_in *local_cli_sa, 
	const struct ip_addr **addr, int *prt)
{
  if (name) {
    free(address.addr);
    struct ip_addr ad = solve_hostname(name);
    memcpy((void *) &address, (void *) &ad, sizeof(ad));
  }

  *addr = &address;
  *prt = port;

  return 0;
}

/* Eof: dst_addr.cc */
