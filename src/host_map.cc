/*
  host_map.cc

  $Id: host_map.cc,v 1.3 2001/09/21 13:50:16 evertonm Exp $
 */

#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "portfwd.h"
#include "host_map.hpp"
#include "solve.h"

void host_map::show() const
{
  iterator<vector<from_addr*>,from_addr*> it(*src_list);
  it.start();
  if (it.cont()) {
    it.get()->show();
    it.next();
  }
  for (; it.cont(); it.next()) {
    syslog(LOG_INFO, ", ");
    it.get()->show();
  }
  syslog(LOG_INFO, " => ");
  dst_addr->show(); 
}

/*
 * Returns -1 on failure; 0 on success.
 */
int host_map::pipe(int sd, const struct ip_addr *ip, int port) const
{
  const struct ip_addr *dst_ip  = dst_addr->get_addr();
  int                  dst_port = dst_addr->get_port();

  const int tmp_len = 32;
  char tmp[tmp_len];

  ONVERBOSE(safe_strcpy(tmp, addrtostr(ip), tmp_len)); 
  ONVERBOSE(syslog(LOG_DEBUG, "TCP pipe: %s:%d => %s:%d", tmp, port, addrtostr(dst_ip), dst_port));

  struct sockaddr_in sa;
  sa.sin_family      = PF_INET;
  sa.sin_port        = htons(dst_port);
  sa.sin_addr.s_addr = *((unsigned int *) dst_ip->addr);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));

  if (connect(sd, (struct sockaddr *) &sa, sizeof(sa))) {
    syslog(LOG_ERR, "pipe: Can't connect to %s:%d: %m", inet_ntoa(sa.sin_addr), dst_port);
    return -1;
  }

  return 0;
}

/*
 * Returns -1 on failure; file descriptor on success.
 */
int host_map::tcp_match(const struct ip_addr *ip, int port) const
{
  iterator<vector<from_addr*>,from_addr*> it(*src_list);
  for (it.start(); it.cont(); it.next()) {
    if (it.get()->match(ip, port))
      return -1;
  }

  return 0;
}

void host_map::udp_forward(const struct sockaddr_in *cli_sa, const struct ip_addr *ip, int port, const char *buf, int buf_len) const
{
  const struct ip_addr *dst_ip  = dst_addr->get_addr();
  int                  dst_port = dst_addr->get_port();

  const int tmp_len = 32;
  char tmp[tmp_len];

  ONVERBOSE(safe_strcpy(tmp, addrtostr(ip), tmp_len)); 
  ONVERBOSE(syslog(LOG_DEBUG, "UDP forward: %s:%d => %s:%d", tmp, port, addrtostr(dst_ip), dst_port));

  int rsd = socket(PF_INET, SOCK_DGRAM, get_protonumber(P_UDP));
  if (rsd == -1) {
    syslog(LOG_ERR, "forward: Can't create UDP socket: %m");
    return;
  }

  /* Allow broadcast packets */
  {
    int one = 1;
    setsockopt(rsd, SOL_SOCKET, SO_BROADCAST, (char *) &one, sizeof(one));
  }

  struct sockaddr_in local_sa;  
   
  /*
   * Perform transparent proxy
   */
  if (transparent_proxy) {

    unsigned int local_sa_len = sizeof(local_sa);

    /*
     * Copy client address
     */
    memcpy(&local_sa, cli_sa, local_sa_len);

    ONVERBOSE(syslog(LOG_ERR, "host_map::udp_forward: Transparent proxy - \"Binding\" to local address: %s:%d", inet_ntoa(local_sa.sin_addr), ntohs(local_sa.sin_port)));
  }

  /*
   * Destination address
   */
  struct sockaddr_in sa;
  sa.sin_family      = PF_INET;
  sa.sin_port        = htons(dst_port);
  sa.sin_addr.s_addr = *((unsigned int *) dst_ip->addr);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));
  
  if (transparent_proxy) memcpy((char *) sa.sin_zero, &local_sa, 8);
  
  int wr = sendto(rsd, buf, buf_len, (transparent_proxy ? MSG_PROXY : 0),
                  (struct sockaddr *) &sa, sizeof(sa));
  if (wr < 0)
    syslog(LOG_ERR, "forward: sendto failed: %m");
  else if (wr < buf_len)
    syslog(LOG_ERR, "forward: Partial write on sendto: %m");

  if (close(rsd))
    syslog(LOG_WARNING, "forward: close on socket failed: %m");
}

/*
 * -1 = match; 0 = miss.
 */
int host_map::udp_match(const struct ip_addr *ip, int port, const char *buf, int buf_len) const
{
  iterator<vector<from_addr*>,from_addr*> it(*src_list);
  for (it.start(); it.cont(); it.next())
    if (it.get()->match(ip, port))
      return -1;

  return 0;
}

/* Eof: host_map.cc */
