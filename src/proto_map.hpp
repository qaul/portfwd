/*
  proto_map.hpp

  $Id: proto_map.hpp,v 1.2 2001/11/19 19:34:44 evertonm Exp $
 */

#ifndef PROTO_MAP_HPP
#define PROTO_MAP_HPP

#include <stdio.h>
#include "host_map.hpp"
#include "solve.h"

class proto_map
{
private:
  vector<int>       *port_list;
  vector<host_map*> *map_list;
  int               ftp_actv;
  int               ftp_pasv;
  struct ip_addr    actv_ip;
  struct ip_addr    pasv_ip;
  int               uid;
  int               gid;
  struct ip_addr    local_listen;
  struct ip_addr    local_source;
  struct ip_addr    *local_src;

public:
  proto_map(vector<int> *port_l, vector<host_map*> *map_l, struct ip_addr *actv, struct ip_addr *pasv, int user, int group, struct ip_addr listen, struct ip_addr *source);

  void show() const;

  void serve(proto_t proto) const;
};

#endif /* PROTO_MAP_HPP */

/* Eof: proto_map.hpp */
