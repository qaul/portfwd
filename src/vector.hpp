// vector.hpp
//
// $Id: vector.hpp,v 1.5 2005/06/28 21:46:27 evertonm Exp $


#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <syslog.h>
#include <stdlib.h>
#include <iostream>
#include "iterator.hpp"
#include "util.hpp"


template <typename T> class vector;

template <typename T> std::ostream& operator << (std::ostream& out, const vector<T>& v);

template <class T>
class vector
{
protected:

  int capacity;
  T   *data;
  T   *past_end;

public:

  vector(int cap = 1);
  vector(const vector& v);
  virtual ~vector();

  const vector& operator = (const vector& v);

  void push(const T& info);
  T    pop();
  void trim();
  void erase();

  int get_capacity() const;
  int get_size() const;
  T&  get_at(int ind) const;
  int is_empty() const;

  T& top();

  void cut(T& info);
  void cut_at(int size);

  void drop(T& info);
  void drop_at(int size);

  T *begin_ptr() const;
  T *past_end_ptr() const;
  static T *next_ptr(T *i);

  friend std::ostream& operator << <T> (std::ostream& out, const vector<T>& v);
};

template <class T>
vector<T>::vector(int cap)
{
  if (cap < 1) {
    syslog(LOG_EMERG, "%s: %s\n", "vector::vector(int)", "capacity must be a positive value");
    exit(1);
  }
  
  capacity = cap;
  data     = safe_new(data, cap);
  past_end = data;
}

template <class T>
vector<T>::vector(const vector& v)
{
  capacity = v.get_size();
  data     = safe_new(data, capacity);
  past_end = data + capacity;
      
  T *src = v.data;
  T *trg = data;
  while (trg < past_end)
    *trg++ = *src++;
}

template <class T>
inline vector<T>::~vector()
{
  delete [] data;
}

template <class T> 
const vector<T>& vector<T>::operator = (const vector& v)
{
  if (capacity < v.get_size())
  {
    delete [] data;
    capacity = v.get_size();
    data     = safe_new(data, capacity);
    past_end = data + capacity;
  }

  T *src = v.data;
  T *trg = data;
  while (trg < past_end)
    *trg++ = *src++;

  return v;
}

template <class T>
void vector<T>::push(const T& info)
{
  int size = get_size();
  if (size >= capacity)
    {
      capacity <<= 1;
      T* new_data = safe_new(data, capacity);
      past_end = new_data + size;

      T *src = data;
      T *trg = new_data;
      while (trg < past_end)
	*trg++ = *src++;
      
      delete [] data;
      data = new_data;
    }

  *(past_end++) = info;      
}

template <class T>
inline T vector<T>::pop()
{
  if (past_end <= data) {
    syslog(LOG_EMERG, "vector::pop(): underflow: size: %d \r\n", get_size());
    exit(1);
  }

    return *--past_end;
}

template <class T>
void vector<T>::trim()
{
  int size = get_size();
  if (size == capacity)
    return;
  
  T* tight_data = safe_new(data, size);
  past_end = tight_data + size;

  T *src = data;
  T *trg = tight_data;
  while (trg < past_end)
    *trg++ = *src++;

  delete [] data;
  data = tight_data;
  capacity = size;
}

template <class T>
inline void vector<T>::erase()
{
    past_end = data;
}

template <class T>
inline int vector<T>::get_capacity() const
{
  return capacity;
}

template <class T>
inline int vector<T>::get_size() const
{
  return past_end - data;
}

template <class T>
inline T& vector<T>::get_at(int ind) const
{
  if ((ind < 0) || (data + ind >= past_end)) {
    syslog(LOG_EMERG, "%s: %s: %d (size: %d)\n", "vector::get_at()", "index out of bounds", ind, get_size());
    exit(1);
  }

  return data[ind];
}

template <class T>
inline int vector<T>::is_empty() const
{
    return past_end == data;
}

template <class T>
inline T& vector<T>::top()
{
  if (past_end <= data) {
    syslog(LOG_EMERG, "vector::top(): empty\r\n");
    exit(1);
  }
  return *(past_end - 1);
}

template <class T>
inline void vector<T>::cut(T& info)
{
    while (*--past_end != info)
	;
}

template <class T>
inline void vector<T>::cut_at(int size)
{
  T *p = data + size;

  if ((size < 0) || (p > past_end)) {
    syslog(LOG_EMERG, "%s: %s: %d (old size: %d)\n", "vector::cut_at()", "new size out of bounds", size, get_size());
    exit(1);
  }

  past_end = p;
}

template <class T>
inline void vector<T>::drop(T& info)
{
  T *p = past_end;

  while (*--p != info)
    ;

  *p = *--past_end;
}

template <class T>
inline void vector<T>::drop_at(int ind)
{
  T *i = data + ind;

  if ((ind < 0) || (i >= past_end)) {
    syslog(LOG_EMERG, "%s: %s: %d (size: %d)\n", "vector::drop_at()", "index out of bounds", ind, get_size());
    exit(1);
  }

  *i = *--past_end;
}

template <class T>
inline T *vector<T>::begin_ptr() const
{
  return data;
}

template <class T>
inline T *vector<T>::past_end_ptr() const
{
  return past_end;
}

template <class T>
inline T *vector<T>::next_ptr(T *i)
{
  return ++i;
}

template <class T>
std::ostream& operator << (std::ostream& out, const vector<T>& v)
{
  out << "(" << v.get_size() << "," << v.get_capacity() << ")";

  iterator<vector<T>, T> it(v);
  for (it.start(); it.cont(); it.next())
    out << " " << it.get();

  return out;
}


#endif   // VECTOR_HPP
