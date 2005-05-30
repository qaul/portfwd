/*
  entry.cc

  $Id: entry.cc,v 1.2 2005/05/30 02:13:28 evertonm Exp $
 */

#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "iterator.hpp"
#include "entry.hpp"
#include "portfwd.h"
#include "forward.h"
#include "fd_set.h"

void grandchild_reaper(int sig)
{
  int status;

  pid_t gchild_pid = wait(&status);

  if (gchild_pid == -1) {
    syslog(LOG_ERR, "child: Error waiting pid of grandchild: %m");
    return;
  }
  
  for (int fd = 0; fd < PORTFWD_MAX_FD; ++fd)
    if (grandchild_pid[fd] == gchild_pid)
      grandchild_pid[fd] = -1;

  if (WIFEXITED(status)) {
    ONVERBOSE(syslog(LOG_WARNING, "child: Grandchild with PID %d exited normally with exit status: %d\n", gchild_pid, WEXITSTATUS(status)));
    return;
  }

  if (WIFSIGNALED(status))
    syslog(LOG_NOTICE, "child: Grandchild received signal: %d\n", WTERMSIG(status)); 
    
  syslog(LOG_ERR, "child: Grandchild with PID %d exited abnormally\n", gchild_pid);
}

void entry::show() const
{
  syslog(LOG_INFO, "%s {\n", get_protoname(proto));
  iterator<vector<proto_map*>,proto_map*> it(*proto_list);
  it.start();
  if (it.cont()) {
    it.get()->show();
    it.next();
  }
  for (; it.cont(); it.next()) {
    syslog(LOG_INFO, "\n");
    it.get()->show();
  }
  syslog(LOG_INFO, "}\n");
}

void entry::spawn(const proto_map *map) const
{
  pid_t pid = fork();
  if (pid) {
    if (pid < 0)
      syslog(LOG_ERR, "fork() failed: %m");

    return;
  }

  /* 
   * Child 
   */

  void (*prev_handler)(int);
  prev_handler = signal(SIGPIPE, SIG_IGN);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "child: signal() failed for SIGPIPE blocking: %m");
    return;
  }

  map->serve(proto);

  syslog(LOG_ERR, "Child exiting (!)\n");
  exit(1);
}

void entry::serve() const
{
  if (!proto_list) {
    syslog(LOG_WARNING, "No protocol entries\n");
    return;
  }

  void (*prev_handler)(int);
  prev_handler = signal(SIGCHLD, grandchild_reaper);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "signal() failed for grandchild handler: %m");
    return;
  }

  iterator<vector<proto_map*>,proto_map*> it(*proto_list);
  for (it.start(); it.cont(); it.next())
    spawn(it.get());
}

/* Eof: entry.cc */
