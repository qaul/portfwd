/*
  to_addr.cc

  $Id: to_addr.cc,v 1.3 2001/07/11 23:59:56 evertonm Exp $
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

const struct ip_addr *to_addr::get_addr() const
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
