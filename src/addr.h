/*
  addr.h

  $Id: addr.h,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#ifndef ADDR_H
#define ADDR_H

#include <stdio.h>

struct ip_addr {
  char  *addr;
  short len;
};

extern const int addr_len;

const char *addrtostr(const struct ip_addr *ip);
void show_addr(const struct ip_addr *ip);

#endif /* ADDR_H */

/* Eof: addr.h */
