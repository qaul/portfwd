/*
  solve.cc

  $Id: solve.cc,v 1.3 2002/05/06 03:02:40 evertonm Exp $
 */

#include <stdlib.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
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

/*
 * Return -1 on error.
 */
int solve_portnumber(const char *portname, const char *protoname) {

  if (isdigit(*portname))
    return atoi(portname);

  struct servent *se;
  se = getservbyname(portname, protoname);
  if (!se)
    return -1;

  return ntohs(se->s_port);
}

int solve_port(const char *portname, const char *protoname) {

  int portnumber = solve_portnumber(portname, protoname);
  if (portnumber == -1) {
    syslog(LOG_ERR, "Failure solving port name: %s/%s\n", portname, protoname);
    exit(1);
  }

  return portnumber;
}

int solve_hostname_addr(char *buf, size_t *buf_len, const char *hostname)
{
  /*
   * Find address
   */
  struct hostent *he;
  he = gethostbyname(hostname);
  if (!he)
    return 1; /* Can't solve hostname */

  int blen = *buf_len; /* save buffer length */

  int addr_len = he->h_length; 

  *buf_len = addr_len; /* return address length */

  if (addr_len > blen)
    return 2; /* Insufficient buffer size */

  /*
   * Return address
   */
  memcpy(buf, *he->h_addr_list, addr_len);

  return 0;
}

struct ip_addr solve_hostname(const char *hostname) {
  const int BUF_SZ = 32;
  char buf[BUF_SZ];
  size_t buf_len = BUF_SZ;
  
  int result = solve_hostname_addr(buf, &buf_len, hostname);
  if (result) {
    switch (result) {
    case 1:
      syslog(LOG_ERR, "solve_hostname(%s): Can't solve hostnames\n", hostname);
      break;
    case 2:
      syslog(LOG_ERR, "solve_hostname(%s): Insufficient buffer space for hostname address (buf_len=%d, addr_len=%d)\n", hostname, BUF_SZ, buf_len);
      break;
    default:
      syslog(LOG_ERR, "solve_hostname(%s): Unexpected error: %d", hostname, result);
    }
    exit(1);
  }
  
  struct ip_addr ip;
  ip.len = buf_len;
  ip.addr = (char *) malloc(ip.len);
  if (!ip.addr) {
    syslog(LOG_ERR, "Can't allocate memory for address: %d bytes\n", ip.len);
    exit(1);
  }

  memcpy(ip.addr, buf, ip.len);

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
