/*
  to_addr.cc

  $Id: to_addr.cc,v 1.4 2002/04/12 21:22:30 evertonm Exp $
 */

#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "addr.h"
#include "to_addr.hpp"
#include "solve.h"

void to_addr::show() const
{
  show_addr(&address);
  syslog(LOG_INFO, ":%d", port);
}

const struct ip_addr *to_addr::get_addr()
{
  if (name) {
    free(address.addr);
    struct ip_addr ad = solve_hostname(name);
    memcpy((void *) &address, (void *) &ad, sizeof(ad));
  }

  return &address;
}

int to_addr::get_port() const
{
  return port;
}

/* Eof: to_addr.cc */
