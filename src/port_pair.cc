/*
  port_pair.cc

  $Id: port_pair.cc,v 1.1 2001/05/15 00:24:59 evertonm Exp $
 */

#include <syslog.h>
#include "port_pair.h"

port_pair::port_pair(int p1, int p2)
{
  first = p1;
  last  = p2;
}

void port_pair::show() const
{
  syslog(LOG_INFO, "%d+%d", first, last);
}

int port_pair::match(int port) const
{
  return (port >= first) && (port <= last);
}

/* Eof: port_pair.cc */
