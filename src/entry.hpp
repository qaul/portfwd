/*
  entry.hpp

  $Id: entry.hpp,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#ifndef ENTRY_HPP
#define ENTRY_HPP

#include "solve.h"
#include "proto_map.hpp"

class entry 
{
private:
  proto_t            proto;
  vector<proto_map*> *proto_list;

  void spawn(const proto_map *map) const;

public:
  entry(proto_t pro, vector<proto_map*> *pro_l) 
    {
      proto      = pro;
      proto_list = pro_l;
    }

  void show() const;

  void serve() const;
};

#endif /* ENTRY_HPP */

/* Eof: entry.hpp */
