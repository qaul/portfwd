/*
  Detect MSG_PROXY symbol.

  $Id: have_msg_proxy.c,v 1.1 2002/04/15 04:15:51 evertonm Exp $
 */

#include <sys/socket.h>

int main()
{
#ifdef MSG_PROXY
  exit(0);
#else
  exit(1);
#endif
}
