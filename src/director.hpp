/*
  director.hpp
  
  $Id: director.hpp,v 1.4 2002/05/07 03:15:36 evertonm Exp $
 */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "to_addr.hpp"

class director : public to_addr
{
private:
  char *args;
  int fd[2];
  pid_t child;
  struct ip_addr address;
  size_t address_buf_size;

  void kill_child();
  void close_sockets();
  void run(char *argv[]);
  int spawn();

public:
  director(const char *str);

  void show() const;

  int get_addr(const char *protoname, 
	const struct sockaddr_in *cli_sa, 
	const struct sockaddr_in *local_cli_sa, 
	const struct ip_addr **addr, int *prt);
};

#endif /* DIRECTOR_HPP */

/* Eof: director.hpp */
