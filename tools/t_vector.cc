/*
  t_vector.c

  $Id: t_vector.cc,v 1.2 2002/09/07 02:27:51 evertonm Exp $
 */


#include <stdlib.h>
#include <iostream>

#include "vector.hpp"
#include "iterator.hpp"

template <class T>
void show(const char *name, const vector<T>& v)
{
  std::cout << "  " << name << " = ";
  std::cout << v;
  std::cout << "\n";
}

int main()
{
    const int MAX = 5;
    
    vector<int> v;

    std::cout << "inserindo elementos em v: ";
    for (int i = 0; i < MAX; ++i) {
      int j = i + 5;
	v.push(j);
	std::cout << j << " ";
    }
    std::cout << "\n";

    show("v", v);
    std::cout << "fazendo v.trim():\n";
    v.trim();
    show("v", v);

    vector<vector<int> > vv;

    vv.push(v);
    vv.push(v);

    std::cout << "vetor de vetores: fazendo vv.push(v) (2x):\n";
    show("vv", vv);
    
    std::cout << "linha 1, coluna 0: vv[1][0] = " << vv.get_at(0).get_at(1) << "\n";

    vector<int> u(v);

    std::cout << "copia obtida por construtor: u(v)\n";
    show("u", u);

    vector<int> t;
    t = u;

    std::cout << "copia obtida por atribuicao: t = u\n";
    show("t", t);

    std::cout << "fazendo t.erase():\n";
    t.erase();
    show("t", t);

    std::cout << "limpando u:\n";

    for (int i = 0; i < MAX; ++i)
	u.pop();

    show("u", u);

    return 0;
}
