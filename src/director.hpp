/*
  director.hpp
  
  $Id: director.hpp,v 1.3 2002/05/06 03:02:40 evertonm Exp $
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
  int get_addr(const char *protoname, const struct ip_addr **addr, int *prt);
};

#endif /* DIRECTOR_HPP */

/* Eof: director.hpp */
