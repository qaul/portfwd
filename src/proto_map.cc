/*
  proto_map.cc

  $Id: proto_map.cc,v 1.1 2001/05/15 00:24:59 evertonm Exp $
 */

#include <syslog.h>
#include <errno.h>
#include "proto_map.hpp"
#include "forward.h"
#include "portfwd.h"

proto_map::proto_map(vector<int> *port_l, vector<host_map*> *map_l, struct ip_addr *actv, struct ip_addr *pasv, int user, int group, struct ip_addr local)
{
  port_list  = port_l;
  map_list   = map_l;
  ftp_actv   = (int) actv;
  if (ftp_actv)
    actv_ip = *actv;
  ftp_pasv   = (int) pasv;
  if (ftp_pasv)
    pasv_ip = *pasv;
  uid        = user;
  gid        = group;
  local_bind = local;
}

void proto_map::show() const
{
  iterator<vector<int>,int> it(*port_list);
  it.start();
  if (it.cont()) {
    syslog(LOG_INFO, "%d", it.get());
    it.next();
  }
  for (; it.cont(); it.next())
    syslog(LOG_INFO, ", %d", it.get());

  syslog(LOG_INFO, " /* uid: %d, gid: %d, bind: %s */", uid, gid, addrtostr(&local_bind));

  if (ftp_actv)
    syslog(LOG_INFO, " ftp-active-mode-on %s", addrtostr(&actv_ip));

  if (ftp_pasv)
    syslog(LOG_INFO, " ftp-passive-mode-on %s", addrtostr(&pasv_ip));

  syslog(LOG_INFO, "\n{");
  iterator<vector<host_map*>,host_map*> it2(*map_list);
  it2.start();
  if (it2.cont()) {
    it2.get()->show();
    it2.next();
  }
  for (; it2.cont(); it2.next()) {
    syslog(LOG_INFO, ";\n");
    it2.get()->show();
  }
  syslog(LOG_INFO, "\n}\n");
}

void proto_map::serve(proto_t proto) const
{
  if (!port_list) {
    syslog(LOG_WARNING, "Missing port list");
    return;
  }

  const struct ip_addr *lcl_bnd = &local_bind;

  switch (proto) {
  case P_TCP:
    tcp_forward(lcl_bnd, port_list, map_list, ftp_actv ? &actv_ip : 0, ftp_pasv ? &pasv_ip : 0, uid, gid);
    break;

  case P_UDP:
    udp_forward(lcl_bnd, port_list, map_list, uid, gid);
    break;

  default:
    syslog(LOG_ERR, "Unknown protocol identifier: %d\n", proto);
  }

}

/* Eof: proto_map.cc */
