/*
  host_map.hpp

  $Id: host_map.hpp,v 1.3 2002/04/12 21:22:30 evertonm Exp $
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
  vector<to_addr*>   *dst_list;
  int next_dst_index;

public:
  host_map(vector<from_addr*> *src, vector<to_addr*> *dst)
    {
      src_list = src;
      dst_list = dst;
      next_dst_index = 0;
    }

  void show() const;

  int pipe(int sd, const struct ip_addr *ip, int port);
  void udp_forward(const struct ip_addr *source, const struct sockaddr_in *cli_sa, const struct ip_addr *ip, int port, const char *buf, int buf_len);

  int tcp_match(const struct ip_addr *ip, int port) const;
  int udp_match(const struct ip_addr *ip, int port, const char *buf, int buf_len) const;
};

#endif /* HOST_MAP_HPP */

/* Eof: host_map.hpp */
