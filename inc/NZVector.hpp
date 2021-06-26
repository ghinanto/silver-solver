// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
//
// Definisce un vettore utilizzando un sistema di storage particolarmente
// efficiente per valori quasi tutti nulli.
// Si parla di elenco dei VALORI riferendosi all'attributo 'val_', elenco
// degli INDICI riferendosi all'attributo 'idx_', e elenco ESTESO riferendosi
// alla forma estesa del vettore in cui vengono elencati anche i valori nulli.
// es. elenco esteso       = (1,0,0,4,9,0,0,0,2,0,0)
//     elnco dei valori    = (1,4,9,2) = val_
//     elenco degli indici = (0,3,4,8,10) = idx_
// L'elenco degli indici termina con un indice di controllo pari alla lunghezza
// dell'elenco esteso.
//
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
  // noexcept necessario in modo che quando un std::vector<NZVector>
  // cresce e rialloca memoria, chiama il costruttore di move e non quello di
  // copia
  NZVector(NZVector&&) noexcept;
  NZVector(const std::initializer_list<T>&);
  // Costruisce assegnando un valore alla capacità
  NZVector(std::size_t);
  // Costruisce usando i coefficienti contenuti in un file
  NZVector(std::string const& file_name);
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

  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco esteso
  T at(const std::size_t pos) const;
  // Restituisce il valore che corrisponde all'indice 'pos' nell'elenco dei
  // valori
  T at_nz(const std::size_t pos) const;
  // Restituisce la lunghezza dell'elenco esteso
  std::size_t size() const;
  // Restituisce la lunghezza dell'elenco dei valori
  std::size_t size_nz() const;
  // Restituisce il massimo numero di valori non nulli che il vettore può
  // contenere
  std::size_t max_size_nz() const;
  // Restituisce la capacità del vettore
  std::size_t capacity_nz() const;
  // Restituisce la posizione nell'elenco dei valori del coefficiente che si
  // trova nella posizione 'pos' nell'elenco esteso.
  // Restituisce '-1' se il coefficiente non fa parte dell'elenco dei valori
  // perchè nullo.
  // es. val_ = {7.3, 4.5}
  //     idx_ = {1, 3, 6}
  //     vettore = {0, 7.3, 0, 4.5, 0, 0}
  //     vettore.plain_to_nonzero(5) == -1
  //     vettore.plain_to_nonzero(3) == 2
  long plain_to_nonzero(const std::size_t pos) const;
  // Restituisce la posizione nell'elenco esteso del coefficiente che si trova
  // nella posizione 'pos' nell'elenco dei valori.
  // es. val_ = {7.3, 4.5}
  //     idx_ = {1, 3, 6}
  //     vettore = {0, 7.3, 0, 4.5, 0, 0}
  //     vettore.nonzero_to_plain(0) == 1
  //     vettore.nonzero_to_plain(1) == 3
  long nonzero_to_plain(const std::size_t pos) const;
  // Scrive sull'output l'elenco degli indici e l'elenco dei valori
  void print(std::ostream& = std::cout) const;

  // Distruttore
  ~NZVector();

 private:
  std::vector<long> idx_{0};
  std::vector<T> val_;
};

#include "../src/NZVector.inl"
#endif  // NZVECTOR_HPP