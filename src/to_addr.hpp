/*
  to_addr.hpp
  
  $Id: to_addr.hpp,v 1.4 2002/05/05 08:55:52 evertonm Exp $
 */

#ifndef TO_ADDR_HPP
#define TO_ADDR_HPP

#include "addr.h"

class to_addr
{
public:
  virtual void show() const = 0;
  virtual int get_addr(const struct ip_addr **addr, int *prt) = 0;
};

#endif /* TO_ADDR_HPP */

/* Eof: to_addr.hpp */
