/*
  solve.h

  $Id: solve.h,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#ifndef SOLVE_H
#define SOLVE_H

#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include "addr.h"

enum proto_t { P_TCP, P_UDP };

extern const int FIRST_PORT;
extern const int LAST_PORT;

const char *get_protoname(proto_t proto);
void solve_protonumbers();
int get_protonumber(proto_t proto);
int solve_port(const char *portname, const char *protoname);
struct ip_addr solve_hostname(const char *hostname);
uid_t solve_user(const char *username);
gid_t solve_group(const char *groupname);

#endif /* SOLVE_H */

/* Eof: solve.h */

