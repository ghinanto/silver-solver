#ifndef NZVECTOR_HPP
#define NZVECTOR_HPP

#include <algorithm>
#include <complex>
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
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
  // Cambia il valore del coefficiente che corrisponde all'indice 'pos'
  // nell'elenco esteso
  void set(const size_t, const T&);
  // Modifica il valore del coefficiente che corrisponde all'indice 'pos'
  // nell'elenco esteso
  // X corrisponde al tipo di un puntatore a funzione, una espressione lambda
  // o una funzione oggetto (functor) che prenda PER RIFERIMENTO NON COSTANTE
  // un solo argomento di tipo double, ovvero il coefficiente.
  // es. double correction{3.14};
  //     vec.set(0, [&](double& val){ val += correction});
  template <std::invocable<T&> X>
  void set(const size_t pos, X);
  // Aggiunge valori alla fine del vettore
  void push_back(const T&);
  // cancella il contenuto del vettore. lascia invariata la capacità
  void clear();

  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco esteso
  T at(const size_t pos) const;
  // Restituisce la lunghezza dell'elenco esteso
  size_t size() const;
  // Scrive sull'output l'elenco degli indici e l'elenco dei valori
  void print(ostream& = cout) const;
  // Scrive sull'output l'elenco esteso
  void print_plain(ostream& = cout) const;
  // Definizione necessaria in modo che ogni specializzazione della classe
  // definisca il proprio operatore
  /*   friend ostream& operator<<(ostream& out, const NZVector&)
    {
      return out;
    }
   */

  // Distruttore
  ~NZVector();

 private:
  // Restituisce la posizione nell'elenco dei valori del coefficiente che si
  // trova nella posizione 'pos' nell'elenco esteso.
  // es. val_ = {7.3, 4.5}
  //     idx_ = {1  , 3  , 6}
  //     vettore = {0, 7.3, 0, 4.5, 0, 0}
  //     vettore.plain_to_non_zero(5) == -1
  //     vettore.plain_to_non_zero(3) == 2
  // L'indice di controllo non corrisponde a nessun coefficiente
  //     vettore.plain_to_non_zero(6) == -1
  long plain_to_nonzero(const size_t) const;
  // Individua l'indice 'pos' nell'elenco degli indici idx_ e restituisce la
  // posizione in cui si trova.
  // Se 'pos' non è presente nell'elenco degli indici, restituisce -1
  vector<long> idx_{0};
  vector<T> val_;
};

#include "../src/NZVector.inl"
#endif  // NZVECTOR_HPP