/*
  addr.c

  $Id: addr.cc,v 1.1 2001/05/15 00:25:03 evertonm Exp $
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include "addr.h"

const int addr_len = 4;

const char *addrtostr(const struct ip_addr *ip)
{
  return inet_ntoa(*((struct in_addr *) ip->addr));
}

void show_addr(const struct ip_addr *ip)
{
  syslog(LOG_INFO, "%s", addrtostr(ip));
}

/* Eof: addr.c */
