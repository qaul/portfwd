/*
  to_addr.cc

  $Id: to_addr.cc,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#include <syslog.h>
#include "util.h"
#include "addr.h"
#include "to_addr.hpp"

void to_addr::show() const
{
  show_addr(&address);
  syslog(LOG_INFO, ":%d", port);
}

const struct ip_addr *to_addr::get_addr() const
{
  return &address;
}

int to_addr::get_port() const
{
  return port;
}

/* Eof: to_addr.cc */
