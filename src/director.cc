/*
  director.cc

  $Id: director.cc,v 1.1 2002/05/05 08:55:52 evertonm Exp $
 */

#include <syslog.h>

#include "util.h"
#include "addr.h"
#include "director.hpp"

director::director(const char *str)
{
  args = safe_strdup(str);
}

void director::show() const
{
  syslog(LOG_INFO, "[%s]", args);
}

int director::get_addr(const struct ip_addr **addr, int *prt)
{
  return -1;
}

/* Eof: dst_addr.cc */
