/*
  director.hpp
  
  $Id: director.hpp,v 1.2 2002/05/05 10:53:12 evertonm Exp $
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

  void kill_child();
  void close_sockets();
  void run(char *argv[]);
  int spawn();

public:
  director(const char *str);

  void show() const;
  int get_addr(const struct ip_addr **addr, int *prt);
};

#endif /* DIRECTOR_HPP */

/* Eof: director.hpp */
