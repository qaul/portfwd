/*
  portfwd.h

  $Id: portfwd.h,v 1.3 2002/04/15 04:15:51 evertonm Exp $
 */

#ifndef PORTFWD_H
#define PORTFWD_H

#include "config.h"

#define MAX_FD 256

extern const int BUF_SZ;
extern const char * const portfwd_version;

#ifdef HAVE_MSG_PROXY
extern int transparent_proxy;
#endif

extern int on_the_fly_dns;

#endif /* PORTFWD_H */

/* portfwd.h */
