/*
  port_pair.h

  $Id: port_pair.h,v 1.1 2001/05/15 00:25:00 evertonm Exp $
 */

#ifndef PORT_PAIR_H
#define PORT_PAIR_H

#include <stdio.h>

class port_pair {
 private:
  int first;
  int last;

 public:
  port_pair(int p1, int p2);

  void show() const;

  int match(int port) const;
};

#endif /* PORT_PAIR_H */

/* Eof: port_pair.h */
