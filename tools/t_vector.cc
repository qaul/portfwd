/*
  t_vector.c

  $Id: t_vector.cc,v 1.1 2001/05/15 00:25:13 evertonm Exp $
 */


#include <stdlib.h>
#include <iostream.h>

#include "vector.hpp"
#include "iterator.hpp"

template <class T>
void show(const char *name, const vector<T>& v)
{
  cout << "  " << name << " = ";
  cout << v;
  cout << "\n";
}

int main()
{
    const int MAX = 5;
    
    vector<int> v;

    cout << "inserindo elementos em v: ";
    for (int i = 0; i < MAX; ++i) {
      int j = i + 5;
	v.push(j);
	cout << j << " ";
    }
    cout << "\n";

    show("v", v);
    cout << "fazendo v.trim():\n";
    v.trim();
    show("v", v);

    vector<vector<int> > vv;

    vv.push(v);
    vv.push(v);

    cout << "vetor de vetores: fazendo vv.push(v) (2x):\n";
    show("vv", vv);
    
    cout << "linha 1, coluna 0: vv[1][0] = " << vv.get_at(0).get_at(1) << "\n";

    vector<int> u(v);

    cout << "copia obtida por construtor: u(v)\n";
    show("u", u);

    vector<int> t;
    t = u;

    cout << "copia obtida por atribuicao: t = u\n";
    show("t", t);

    cout << "fazendo t.erase():\n";
    t.erase();
    show("t", t);

    cout << "limpando u:\n";

    for (int i = 0; i < MAX; ++i)
	u.pop();

    show("u", u);

    return 0;
}
