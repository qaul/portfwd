/*
  solve.cc

  $Id: solve.cc,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#include <stdlib.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <syslog.h>
#include <string.h>

#include "util.h"
#include "solve.h"


const int FIRST_PORT = 0;
const int LAST_PORT  = 65535;

const char *protoname_tab[] = 
{
  "tcp",
  "udp",
  0
};

int protonumber_tab[] =
{
  -1,
  -1
};

const char *get_protoname(proto_t proto)
{
  return protoname_tab[proto];
}

void solve_protonumbers()
{
  struct protoent *pe;

  for (int i = 0; protoname_tab[i]; ++i) {
    const char *protoname = get_protoname((proto_t) i);
    pe = getprotobyname(protoname);
    if (!pe) {
      syslog(LOG_ERR, "Failure solving protocol name: %s\n", protoname);
      exit(1);
    }
    protonumber_tab[i] = pe->p_proto;
  }
}

int get_protonumber(proto_t proto)
{
  return protonumber_tab[proto];
}

int solve_port(const char *portname, const char *protoname) {

  if (isdigit(*portname))
    return atoi(portname);

  struct servent *se;
  se = getservbyname(portname, protoname);
  if (!se) {
    syslog(LOG_ERR, "Failure solving port name: %s/%s\n", portname, protoname);
    exit(1);
  }

  return ntohs(se->s_port);
}

struct ip_addr solve_hostname(const char *hostname) {
  struct hostent *he;
  he = gethostbyname(hostname);
  if (!he) {
    syslog(LOG_ERR, "Can't solve hostname: %s\n", hostname);
    exit(1);
  }

  struct ip_addr ip;
  ip.len = he->h_length;
  ip.addr = (char*) malloc(ip.len);
  if (!ip.addr) {
    syslog(LOG_ERR, "Can't allocate memory for address: %d bytes\n", ip.len);
    exit(1);
  }

  memcpy(ip.addr, *he->h_addr_list, ip.len);

  return ip;
}

uid_t solve_user(const char *username)
{
  struct passwd *pwd;

  if (isdigit(*username))
    pwd = getpwuid(atoi(username));
  else
    pwd = getpwnam(username);
  
  if (!pwd) {
    syslog(LOG_ERR, "Can't solve user: %s: %m", username);
    exit(1);
  }

  return pwd->pw_uid;
}

gid_t solve_group(const char *groupname)
{
  struct group *grp;

  if (isdigit(*groupname))
    grp = getgrgid(atoi(groupname));
  else
    grp = getgrnam(groupname);
  
  if (!grp) {
    syslog(LOG_ERR, "Can't solve group: %s: %m", groupname);
    exit(1);
  }

  return grp->gr_gid;
}

/* Eof: solve.cc */
