/*
  $Id: fd_set.cc,v 1.1 2005/05/30 02:13:28 evertonm Exp $
 */


#include <ulimit.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#include "fd_set.h"
#include "util.h"


void close_fds(int first_fds) {

  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    rl.rlim_max = PORTFWD_MAX_FD;
  
  unsigned int fd;
  if (rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = PORTFWD_MAX_FD;

  for (fd = first_fds; fd < rl.rlim_max; ++fd)
    close(fd);

  errno = 0; /* close() on invalid fd */
}

void fdset(int fd, fd_set *fds, int *maxfd)
{
  FD_SET(fd, fds);
  *maxfd = MAX(*maxfd, fd + 1);
}

void fdclear(int fd, fd_set *fds, int *maxfd)
{
  int i;

  FD_CLR(fd, fds);
  
  for (i = *maxfd - 1; i >= 0; --i)
    if (FD_ISSET(i, fds))
      break;

  *maxfd = i + 1;
}


/* eof */
