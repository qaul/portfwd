/*
  director.cc

  $Id: director.cc,v 1.7 2002/05/08 03:50:03 evertonm Exp $
 */

#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


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
  if (child == -1)
    return;

  kill(child, SIGTERM);
  child = -1;
}

void director::run(char *argv[])
{

  ONVERBOSE(syslog(LOG_DEBUG, "Spawning director: %s", argv[0]));

  /*
   * Create unix domain socket
   */
  if (socketpair(AF_LOCAL, SOCK_STREAM, 0, fd)) {
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

    ONVERBOSE(syslog(LOG_INFO, "Delegating child PID %d to init", child));
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
   * Restore default SIGPIPE disposition
   */
  void (*prev_handler)(int);
  prev_handler = signal(SIGPIPE, SIG_DFL);
  if (prev_handler == SIG_ERR)
    syslog(LOG_ERR, "Director child: signal() failed restoring default SIGPIPE disposition: %m");

  ONVERBOSE(syslog(LOG_DEBUG, "Invoking director: %s", argv[0]));

  /*
   * Invoke external director program
   */

  close_fds(2); /* release all file descriptors */

  execv(argv[0], argv);

  syslog(LOG_ERR, "Director child PID %d: fatal: execv(%s) failed: %m", getpid(), argv[0]);
  exit(1);
}

int director::spawn()
{
  int result = -1;
  char **argv = 0;
  char *dir_str = safe_strdup(args);

  ONVERBOSE(syslog(LOG_DEBUG, "Parsing director: %s", args));

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

  result = 0;

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
  address_buf_size = 32;
  address.len = 0;
  address.addr = (char *) malloc(address_buf_size * sizeof(char *));
  if (!address.addr) {
    syslog(LOG_ERR, "director::director(): malloc(%d) failed", address_buf_size);
    exit(1);
  }
}

void director::show() const
{
  syslog(LOG_INFO, "[%s]", args);
}

int director::get_addr(const char *protoname, 
	const struct sockaddr_in *cli_sa, 
	const struct sockaddr_in *local_cli_sa, 
	const struct ip_addr **addr, int *prt)
{
  int fd0 = fd[0];

  if (fd0 == -1) {
    if (spawn())
      syslog(LOG_ERR, "director::get_addr(): spawn() failed");

    return -1;
  }

  int cli_src_port = htons(cli_sa->sin_port);
  int cli_loc_port = htons(local_cli_sa->sin_port);

  const int ADDR_STR_BUF_SZ = 128;
  char cli_src_addr[ADDR_STR_BUF_SZ];
  char cli_loc_addr[ADDR_STR_BUF_SZ];

  safe_strcpy(cli_src_addr, inet_ntoa(cli_sa->sin_addr), ADDR_STR_BUF_SZ);
  safe_strcpy(cli_loc_addr, inet_ntoa(local_cli_sa->sin_addr), ADDR_STR_BUF_SZ);

  /*
   * Write query
   */

  const int WR_BUF_SZ = 128;
  char wr_buf[WR_BUF_SZ];

  int len = snprintf(wr_buf, WR_BUF_SZ, "%s %s %d %s %d\n", protoname, cli_src_addr, cli_src_port, cli_loc_addr, cli_loc_port);
  if (len == -1) {
    syslog(LOG_ERR, "director::get_addr() failed due to snprintf() overflow");
    return -1;
  }

  int wr = write(fd0, wr_buf, len);
  if (wr != len) {
    if (wr == -1) {
      switch (errno) {
      case EBADF:
      case EINVAL:
      case EPIPE:
	syslog(LOG_ERR, "director::get_addr(): finishing child: can't write to external director: %m");
	close_sockets();
	kill_child();
	break;
      default:
	syslog(LOG_ERR, "director::get_addr(): skipping: can't write to external director: %m");
      }

      return -1;
    }
  }

  /*
   * Read response
   */

  const int RD_BUF_SZ = 128;
  char rd_buf[RD_BUF_SZ];

  int rd = read(fd0, rd_buf, RD_BUF_SZ);
  if (rd == -1) {
    switch (errno) {
      case EBADF:
      case EINVAL:
	syslog(LOG_ERR, "director::get_addr(): finishing child: can't read from external director: %m");
	close_sockets();
	kill_child();
	break;
    default:
	syslog(LOG_ERR, "director::get_addr(): skipping: can't read from external director: %m");
    }

    return -1;
  }

  if (rd == 0) {
    syslog(LOG_ERR, "director::get_addr(): can't read from external director: external director closed communication");
    close_sockets();
    kill_child();

    return -1;
  }

  /*
   * Parse response
   */

  const char *SEP = "\r\n\t ";
  char *response = strtok(rd_buf, SEP);
  if (!response) {
    syslog(LOG_ERR, "External director returned null response");
    return -1;
  }
  
  if (!strcmp(response, "reject")) {
    ONVERBOSE(syslog(LOG_INFO, "External director rejected source"));
    return -1;
  }

  if (strcmp(response, "forward")) {
    syslog(LOG_ERR, "External director returned invalid response: %s", response);
    return -1;
  }

  char *remote_host = strtok(0, SEP);
  if (!remote_host) {
    syslog(LOG_ERR, "External director returned null hostname");
    return -1;
  }

  char *remote_port = strtok(0, SEP);
  if (!remote_port) {
    syslog(LOG_ERR, "External director returned null port");
    return -1;
  }

  ONVERBOSE2(syslog(LOG_DEBUG, "External director forwarded to %s:%s", remote_host, remote_port));

  /*
   * Solve response
   */

  int rem_port = solve_portnumber(remote_port, protoname);
  if (rem_port == -1) {
    syslog(LOG_ERR, "Can't resolve port pointed by external director: %s", remote_port);
    return -1;
  }

  const int ADDR_BUF_SZ = 32;
  char addr_buf[ADDR_BUF_SZ];
  size_t addr_buf_len = ADDR_BUF_SZ;

  int result = solve_hostname_addr(addr_buf, &addr_buf_len, remote_host);
  if (result) {
    syslog(LOG_ERR, "Can't resolve hostname pointed by external director: %s", remote_host);
    return -1;
  }

  /*
   * Return address/port
   */

  if (addr_buf_len > address_buf_size) {
    syslog(LOG_ERR, "Insufficient space in local buffer for address (local_buffer_size=%d < address_length=%d)", address_buf_size, addr_buf_len);
    return -1;
  }

  /* Copy address */
  memcpy(address.addr, addr_buf, addr_buf_len);
  address.len = addr_buf_len;

  *addr = &address;
  *prt = rem_port;

  return 0;
}

/* Eof: dst_addr.cc */
