/*
  portfwd.h

  $Id: portfwd.h,v 1.5 2005/05/30 02:13:28 evertonm Exp $
 */

#ifndef PORTFWD_H
#define PORTFWD_H

#include "config.h"

extern const int BUF_SZ;
extern const char * const portfwd_version;

#ifdef HAVE_MSG_PROXY
extern int transparent_proxy;
#endif

extern int on_the_fly_dns;
extern int foreground;

#endif /* PORTFWD_H */

/* portfwd.h */
