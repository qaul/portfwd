/*
  to_addr.hpp
  
  $Id: to_addr.hpp,v 1.3 2002/04/12 21:22:30 evertonm Exp $
 */

#ifndef TO_ADDR_HPP
#define TO_ADDR_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "addr.h"

class to_addr
{
private:
  char           *name;
  struct ip_addr address;
  int            port;

public:
  to_addr(char *hostname, struct ip_addr addr, int prt) 
    {
      name    = hostname;
      address = addr;
      port    = prt;
    }

  void show() const;

  const struct ip_addr *get_addr();
  int get_port() const;
};

#endif /* TO_ADDR_HPP */

/* Eof: to_addr.hpp */
