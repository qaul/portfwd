/*
  iterator.hpp

  $Id: iterator.hpp,v 1.1 2001/05/15 00:25:00 evertonm Exp $
*/


#ifndef ITERATOR_HPP
#define ITERATOR_HPP


template <class C, class T>
class iterator
{
private:

  C *vect;
  T *current;

public:

  iterator(const C& vec);

  void start();
  int  cont();
  void next();

  T& get();
};

template <class C, class T>
iterator<C, T>::iterator(const C& vec)
{
  vect = (C*) &vec;
}

template <class C, class T>
void iterator<C, T>::start()
{
  current = vect->begin_ptr();
}

template <class C, class T>
int iterator<C, T>::cont()
{
  return current != vect->past_end_ptr();
}

template <class C, class T>
void iterator<C, T>::next()
{
  current = C::next_ptr(current);
}

template <class C, class T>
T& iterator<C, T>::get()
{
  return *current;
}


#endif  // ITERATOR_HPP


// Eof: iterator.hpp
