/*
  entry.hpp

  $Id: entry.hpp,v 1.2 2004/01/28 19:14:10 evertonm Exp $
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
  entry(proto_t pro, vector<proto_map*> *pro_l, bool fragile) 
    {
      proto      = pro;
      proto_list = pro_l;

      iterator<vector<proto_map*>,proto_map*> it(*proto_list);
      for (it.start(); it.cont(); it.next())
        it.get()->set_fragile(fragile);
    }

  void show() const;

  void serve() const;
};

#endif /* ENTRY_HPP */

/* Eof: entry.hpp */
