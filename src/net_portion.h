/*
  net_portion.h

  $Id: net_portion.h,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#ifndef NET_PORTION_H
#define NET_PORTION_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "addr.h"

class net_portion
{
 private:
  struct ip_addr address;
  short          mask_len;

 public:
  net_portion(struct ip_addr addr, short m_len);

  void show() const;

  int owns(const struct ip_addr *ip) const;
};

#endif /* NET_PORTION_H */

/* Eof: net_portion.h */
