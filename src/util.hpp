// util.hpp
//
// $Id: util.hpp,v 1.1 2001/05/15 00:25:01 evertonm Exp $


#ifndef UTIL_HPP
#define UTIL_HPP


#include "util.h"


template <class T> 
T* safe_new(T* sample, int size)
{
  sample = new T[size];
  if (!sample) {
    syslog(LOG_EMERG, "%s:%s (%d x %d)\n", "safe_new()", "could not allocate array", size, sizeof(T));
    exit(1);
  }

  return sample;
}


#endif  // UTIL_HPP
