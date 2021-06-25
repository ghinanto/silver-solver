// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
#ifndef NZVECTOR_HPP
#define NZVECTOR_HPP

#include <algorithm>
#include <complex>
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template <class T = double>
class NZVector
{
 public:
  // Costruttori
  NZVector();
  NZVector(const NZVector&);
  // noexcept necessario in modo che quando un vector<NZVector> (i.e. Matrix)
  // cresce e rialloca memoria, chiama il costruttore di move e non quello di
  // copia
  NZVector(NZVector&&) noexcept;
  NZVector(const std::initializer_list<T>&);
  NZVector(std::size_t);
  // NZVector(std::ifstream&);
  NZVector(std::string const&);
  // Operatore uguale
  NZVector& operator=(const NZVector&);
  NZVector& operator=(NZVector&&) noexcept;
  NZVector& operator=(const std::initializer_list<T>&);

  // imposta capacità del vettore
  void reserve(const std::size_t);
  // Cambia il valore del coefficiente che corrisponde all'indice 'pos'
  // nell'elenco esteso
  void set(const std::size_t, const T&);
  // Modifica il valore del coefficiente che corrisponde all'indice 'pos'
  // nell'elenco esteso
  // X corrisponde al tipo di un puntatore a funzione, una espressione lambda
  // o una funzione oggetto (functor) che prenda PER RIFERIMENTO NON COSTANTE
  // un solo argomento di tipo double, ovvero il coefficiente.
  // es. double correction{3.14};
  //     vec.set(0, [&](double& val){ val += correction});
  template <std::invocable<T&> X>
  void set(const std::size_t pos, X);
  // Aggiunge valori alla fine del vettore
  void push_back(const T&);
  // cancella il contenuto del vettore. lascia invariata la capacità
  void clear();

  //
  long nonzero_to_plain(const std::size_t) const;
  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco esteso
  T at(const std::size_t pos) const;
  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco dei
  // valori
  T at_nz(const std::size_t) const;
  // Restituisce la lunghezza dell'elenco esteso
  std::size_t size() const;
  // Restituisce la lunghezza dell'elenco dei valori
  std::size_t size_nz() const;
  // Restituisce il massimo numero di valori non nulli che il vettore può
  // contenere
  std::size_t max_size_nz() const;
  // Restituisce la capacità del vettore
  std::size_t capacity_nz() const;
  // Scrive sull'output l'elenco degli indici e l'elenco dei valori
  void print(std::ostream& = std::cout) const;
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
  long plain_to_nonzero(const std::size_t) const;
  // Individua l'indice 'pos' nell'elenco degli indici idx_ e restituisce la
  // posizione in cui si trova.
  // Se 'pos' non è presente nell'elenco degli indici, restituisce -1
  std::vector<long> idx_{0};
  std::vector<T> val_;
};

#include "../src/NZVector.inl"
#endif  // NZVECTOR_HPP