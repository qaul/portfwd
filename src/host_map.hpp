/*
  host_map.hpp

  $Id: host_map.hpp,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#ifndef HOST_MAP_HPP
#define HOST_MAP_HPP

#include <stdio.h>
#include "vector.hpp"
#include "from_addr.hpp"
#include "to_addr.hpp"

class host_map
{
private:
  vector<from_addr*> *src_list;
  to_addr            *dst_addr;

public:
  host_map(vector<from_addr*> *src, to_addr *dst)
    {
      src_list = src;
      dst_addr = dst;
    }

  void show() const;

  int pipe(int sd, const struct ip_addr *ip, int port) const;
  void udp_forward(const struct sockaddr_in *cli_sa, const struct ip_addr *ip, int port, const char *buf, int buf_len) const;

  int tcp_match(const struct ip_addr *ip, int port) const;
  int udp_match(const struct ip_addr *ip, int port, const char *buf, int buf_len) const;
};

#endif /* HOST_MAP_HPP */

/* Eof: host_map.hpp */
