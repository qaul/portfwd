/*
  to_addr.hpp
  
  $Id: to_addr.hpp,v 1.2 2001/07/08 04:25:48 evertonm Exp $
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

  const struct ip_addr *get_addr() const;
  int get_port() const;
};

#endif /* TO_ADDR_HPP */

/* Eof: to_addr.hpp */
