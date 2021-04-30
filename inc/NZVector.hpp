#ifndef NZVECTOR_HPP
#define NZVECTOR_HPP

#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <class T = double>
class NZVector
{
 public:
  // Costruttori
  NZVector();
  NZVector(const NZVector&);
  NZVector(NZVector&&);
  NZVector(const initializer_list<T>&);
  // Operatore uguale
  NZVector& operator=(const NZVector&);
  NZVector& operator=(NZVector&&);
  NZVector& operator=(const initializer_list<T>&);

  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco esteso
  T at(const size_t pos) const;
  // Restituisce la lunghezza dell'elenco esteso
  size_t size() const;

  void print(ostream& = cout) const;
  void print_plain(ostream& = cout) const;
  // Definizione necessaria in modo che ogni specializzazione della classe
  // definisce il proprio operatore
  friend ostream& operator<<(ostream& out, const NZVector&)
  {
    return out;
  }

  void push_back(const T&);

  // Distruttore
  ~NZVector();

 private:
  vector<long> idx_{0};
  vector<T> val_;
};

#include "../src/NZVector.inl"
#endif  // NZVECTOR_HPP