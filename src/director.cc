/*
  director.cc

  $Id: director.cc,v 1.2 2002/05/05 10:53:12 evertonm Exp $
 */

#include <syslog.h>
#include <string.h>
#include <errno.h>

#include "signal.h"
#include "util.h"
#include "addr.h"
#include "solve.h"
#include "vector.hpp"
#include "director.hpp"

void director::close_sockets()
{
  close(fd[0]);
  close(fd[1]);
  fd[0] = -1;
  fd[1] = -1;
}

void director::kill_child()
{
  kill(child, SIGTERM);
  child = -1;
}

void director::run(char *argv[])
{
  /*
   * Create unix domain socket
   */
  if (socketpair(PF_INET, SOCK_STREAM, get_protonumber(P_TCP), fd)) {
    syslog(LOG_ERR, "Failure creating unix domain socket: %m");
    return;
  }

  pid_t child_pid = fork();
  if (child_pid) {
    /*
     * Parent code
     */

    if (child_pid == -1) {
      syslog(LOG_ERR, "director::run(): fork() failed for temporary child: %m");
      close_sockets();
      return;
    }

    return;
  }

  /*
   * Temporary child code
   */

  child = fork();
  if (child) {
    /*
     * Temporary child
     */
    
    if (child == -1) {
      syslog(LOG_ERR, "director::run(): fork() failed for actual child: %m");
      exit(1);
    }

    ONVERBOSE(syslog(LOG_INFO, "Delegating child %d to init", child_pid));
    exit(0);
  }

  /*
   * Actual child
   */

  /* Attach stdin to fd[1] */
  if (dup2(fd[1], 0) == -1) {
    syslog(LOG_ERR, "Director child: fatal: could not attach stdin to unix domain socket: %m");
    exit(1);
  }

  /* Attach stdout to fd[1] */
  if (dup2(fd[1], 1) == -1) {
    syslog(LOG_ERR, "Director child: fatal: could not attach stdout to unix domain socket: %m");
    exit(1);
  }

  /*
   * Invoke external director program
   */
  execv(argv[0], argv);

  syslog(LOG_ERR, "Director child: fatal: execv(%s) failed: %m", argv[0]);
  exit(1);
}

int director::spawn()
{
  int result = -1;
  char **argv = 0;
  char *dir_str = safe_strdup(args);

  ONVERBOSE(syslog(LOG_DEBUG, "Spawning director: %s", args));

  /*
   * Parse director string into argv
   */

  const char *SEP = "\r\n\t ";

  char *prog_name = strtok(dir_str, SEP);
  if (!prog_name) {
    syslog(LOG_ERR, "Invalid null director!");
    goto clean;
  }

  {
    vector<char*> arg_list;
    arg_list.push(prog_name);
    
    for (;;) {
      char *arg = strtok(0, SEP);
      if (!arg)
	break;
      arg_list.push(arg);
    }
    
    argv = (char **) malloc((arg_list.get_size() + 1) * sizeof(char*));
    if (!argv)
      goto clean;
    
    {
      int i = 0;
      iterator<vector<char*>,char*> it(arg_list);
      for (it.start(); it.cont(); it.next(), ++i)
	argv[i] = it.get();
      argv[i] = 0;
    }
  }

  /*
   * Run external director
   */
  run(argv);

clean:
  free(dir_str);

  if (argv)
    free(argv);

  return result;
}

director::director(const char *str)
{
  args = safe_strdup(str);
  fd[0] = -1;
  fd[1] = -1;
  child = -1;
}

void director::show() const
{
  syslog(LOG_INFO, "[%s]", args);
}

int director::get_addr(const struct ip_addr **addr, int *prt)
{
  if (fd[0] == -1)
    if (spawn())
      return -1;

  /*
   * Write query
   */

  const int BUF_SZ = 128;
  char buf[BUF_SZ];

  int len = snprintf(buf, BUF_SZ, "%s %s\n", "a.a.a.a", "pppp");
  if (len == -1) {
    syslog(LOG_ERR, "director::get_addr() failed due to snprintf() overflow");
    return -1;
  }

  int wr = write(fd[0], buf, len);
  if (wr != len) {
    if (wr == -1) {
      switch (errno) {
      case EBADF:
      case EINVAL:
      case EPIPE:
	syslog(LOG_ERR, "director::get_addr(): finishing child: can't write to external director: %m");
	close_sockets();
	kill_child();
	return -1;
      default:
	syslog(LOG_ERR, "director::get_addr(): skipping: can't write to external director: %m");
	return -1;
      }
    }
  }

  /*
   * Read response
   */

  return -1;
}

/* Eof: dst_addr.cc */
