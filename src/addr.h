/*
  addr.h

  $Id: addr.h,v 1.2 2002/05/05 08:55:52 evertonm Exp $
 */

#ifndef ADDR_H
#define ADDR_H

struct ip_addr {
  char  *addr;
  short len;
};

extern const int addr_len;

const char *addrtostr(const struct ip_addr *ip);
void show_addr(const struct ip_addr *ip);

#endif /* ADDR_H */

/* Eof: addr.h */
