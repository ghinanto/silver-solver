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

  // imposta capacità del vettore
  void reserve(const size_t);
  // cambia valore di un coefficiente
  void set(const size_t, const T&);
  // cancella il contenuto del vettore. lascia invariata la capacità
  void clear();
  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco esteso
  T at(const size_t pos) const;
  // Restituisce la lunghezza dell'elenco esteso
  size_t size() const;

  void print(ostream& = cout) const;
  void print_plain(ostream& = cout) const;
  // Definizione necessaria in modo che ogni specializzazione della classe
  // definisce il proprio operatore
  /*   friend ostream& operator<<(ostream& out, const NZVector&)
    {
      return out;
    }
   */
  void push_back(const T&);

  // Distruttore
  ~NZVector();

  long plain_to_nonzero(const size_t) const;

 private:
  // Individua l'indice 'pos' nell'elenco degli indici idx_ e restituisce la
  // posizione in cui si trova.
  // Se 'pos' non è presente nell'elenco degli indici, restituisce -1
  vector<long> idx_{0};
  vector<T> val_;
};

#include "../src/NZVector.inl"
#endif  // NZVECTOR_HPP