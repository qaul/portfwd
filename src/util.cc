/*
  util.c

  $Id: util.cc,v 1.7 2002/05/07 03:58:12 evertonm Exp $
 */

#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>

#include "util.h"

/* Solaris */
#ifdef HAVE_SYS_TERMIOS_H 
#include <sys/termios.h> 
#endif 

int verbose_mode = 0; /* See ONVERBOSE in util.h */

const char *prog_name = 0;

void set_prog_name(const char *name)
{
  prog_name = name;
}

const char *get_prog_name()
{
  return prog_name ? prog_name : "?";
}

char* safe_strdup(const char* str)
{
  const char* fmt  = "%s: %s\n";
  const char* func = "safe_strdup";

  if (!str) {
    syslog(LOG_EMERG, fmt, func, "null string pointer");
    exit(1);
  }
  char* s = strdup(str);
  if (!s) {
    syslog(LOG_EMERG, fmt, func, "could not allocate string");
    exit(1);
  }

  return s;
}

void safe_strcpy(char *trg, const char *src, int trg_buf_size)
{
  for (char *past_end = trg + trg_buf_size - 1; trg < past_end; ++src, ++trg)
    {
      char c = *src;
      *trg = c;
      if (!c)
        return;
    }
  *trg = '\0';
}

int cd_root() {

  if (chdir("/") == -1) {
    syslog(LOG_WARNING, "cd_root(): can't chdir to / (root): %m");
    return -1;
  }

  return 0;
}

void close_fds() {
  const int LOCAL_MAXFD = 2048;

  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    rl.rlim_max = LOCAL_MAXFD;
  
  unsigned int fd;
  if (rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = LOCAL_MAXFD;

  for (fd = 0; fd < rl.rlim_max; ++fd)
    close(fd);

  errno = 0; /* close() on invalid fd */
}

int std_to_null() {
  /*
   * Conecta stdin, stdout e stderr a /dev/null.
   */
  const char *dev_null = "/dev/null";
  int fd = open(dev_null, O_RDWR);
  if (fd) {
    if (fd < 0)
      syslog(LOG_ERR, "std_to_null(): error opening %s: %m", dev_null);
    else
      syslog(LOG_ERR, "std_to_null(): opening %s: expected fd 0, got %d", dev_null, fd);
    return -1;
  }

  fd = dup(0);
  if (fd != 1) {
    if (fd < 0)
      syslog(LOG_ERR, "std_to_null(): dup(0) failed: %m");
    else
      syslog(LOG_ERR, "std_to_null(): dup(0) expected fd 1, got %d", fd);
    return -1;
  }

  fd = dup(0);
  if (fd != 2) {
    if (fd < 0)
      syslog(LOG_ERR, "std_to_null(): dup(0) failed: %m");
    else
      syslog(LOG_ERR, "std_to_null(): dup(0) expected fd 2, got %d", fd);
    return -1;
  }

  return 0;
}

int daemonize()
{
  ONVERBOSE(syslog(LOG_INFO, "Daemonizing"));
  
  /*
   * Tenta desconectar terminal de controle.
   */
  const char *tty_path = "/dev/tty";
  int tty = open(tty_path, O_NOCTTY);
  if (tty == -1)
    syslog(LOG_WARNING, "daemonize(): can't open tty: %s: %m", tty_path);
  else {
    if (ioctl(tty, TIOCNOTTY))
      syslog(LOG_WARNING, "daemonize(): can't detach terminal: %m");
    if (close(tty))
      syslog(LOG_WARNING, "daemonize(): can't close tty: %s: %m", tty_path);
  }

  /*
   * Garante que futuros open()s nao vao alocar um terminal de controle.
   */
  pid_t pid = fork();
  if (pid) {
    if (pid < 0) {
      syslog(LOG_ERR, "daemonize(): fork() failed");
      return -1;
    }

    /* 
     * Parent exits
     */
    exit(0);
  }
  /*
   * Child proceeds
   */

  return 0;
}

void socket_close(int fd) 
{
  DEBUGFD(syslog(LOG_DEBUG, "socket_close() on socket FD %d", fd));
  if (shutdown(fd, 0)) 
    syslog(LOG_ERR, "socket_close(): shutdown() on socket FD %d failed: %m", fd);
  if (close(fd))
    syslog(LOG_ERR, "socket_close(): close() on socket FD %d failed: %m", fd);
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


/* eof: util.c */
