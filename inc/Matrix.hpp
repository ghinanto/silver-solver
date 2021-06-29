// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
//
// Rappresenta una matrice come un vettore di NZVector, ovvero un vettore che
// implementa uno schema di storage particolarmente efficiente per valori
// quasi tutti nulli.
// La classe è compatibile con gli algoritmi della std library del C++.
// Scopo primario della classe è rappresentare sistemi lineari a coefficienti
// sia reali, sia complessi, e risolverli.
// Per questo i parametri template del metodo 'solve' sono stati vincolati ai
// tipi aritmetici decimali e ai complessi che usino a loro volta tipi
// aritmetici decimali come parametri template.
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <complex>
#include <fstream>
#include <tuple>
#include <vector>
#include "./NZVector.hpp"

template <class T>
class Matrix
{
 public:
  // Costruttori
  Matrix();
  Matrix(Matrix const&);
  Matrix(Matrix&&) noexcept;
  // Costruisce la matrice con i coefficienti contenuti in un file
  Matrix(std::ifstream&);
  Matrix(std::string const& file_name);
  // Costruisce una matrice a valori REALI generati random nell'intervallo
  // compreso tra due valori.
  // Infatti in questo caso il tipo template della classe 'T' corrisponde a un
  // tipo aritmetico
  Matrix(std::size_t rows, std::size_t cols, T first_bound, T second_bound);
  // Costruisce una matrice a valori COMPLESSI le cui parti reali e immaginarie
  // sono generate random nell'intervallo compreso tra due valori.
  // In questo caso il tipo template della classe 'T' corrisponde a
  // 'complex<Arithmetic>'
  template <class Arithmetic>
  Matrix(std::size_t rows,
         std::size_t cols,
         short complex_on_tot,
         Arithmetic first_bound,
         Arithmetic second_bound);
  // Operatore uguale
  Matrix& operator=(Matrix const&);
  Matrix& operator=(Matrix&&) noexcept;

  // La parola chiave 'const' rende l'overload contrassegnato prioritario nel
  // caso di oggetti 'const'. Di conseguenza esso deve restituire un
  // 'const_iterator', ovvero un iteratore incapace di modificare ciò che punta.
  std::vector<NZVector<T>>::iterator begin();
  std::vector<NZVector<T>>::const_iterator begin() const;
  std::vector<NZVector<T>>::iterator end();
  std::vector<NZVector<T>>::const_iterator end() const;

  std::reverse_iterator<typename std::vector<NZVector<T>>::iterator> rbegin();
  std::reverse_iterator<typename std::vector<NZVector<T>>::const_iterator>
  rbegin() const;
  std::reverse_iterator<typename std::vector<NZVector<T>>::iterator> rend();
  std::reverse_iterator<typename std::vector<NZVector<T>>::const_iterator>
  rend() const;

  // Assegna un valore alla capacità della matrice, ovvero il numero righe
  void reserve(std::size_t);
  // Aggiunge righe alla fine della matrice
  void push_back(NZVector<T> const&);
  void push_back(NZVector<T>&&);
  // Costruisce una riga alla fine della matrice passando gli argomenti al
  // costruttore di NZVector
  template <class... Args>
  std::vector<NZVector<T>>::reference emplace_back(Args&&... args);

  // Permette di accedere al contenuto della matrice.
  NZVector<T>& row(std::size_t);

  // Cancella il contenuto della matrice
  void clear();

  //  Restituisce il numero di righe della matrice
  std::size_t rows() const;
  // Restituisce il numero di colonne della matrice
  std::size_t cols() const;

  // Mostra il contenuto della matrice su output.
  void print(std::ostream& = std::cout) const;
  // Scrive la matrice su file
  void to_file(std::string const& file_name) const;
  void to_file(std::ofstream&) const;

  // Ogni incognita è indicizzata partendo da zero in base alla colonna in cui
  // si trova.
  // Restituisce una tuple contente:
  //   [0] = vettore soluzione. Contiene i valori delle inncognite.
  //   [1] = elenco degli indici associati alle incognite
  // Se il sistema è indeterminato, alcune incognite assumono il ruolo di
  // parametro. Quindi ogni soluzione elenca anche i valori dei coefficienti
  // dei parametri, in ordine crescente dell'indice dei parametri.
  // es. sistema 2 eq, 4 incognite
  //     auto sol = mat.solve(terms);
  //     std::get<0>(sol) è {{1., 0.4, -4.3}, {3.2, -0.1, 0.6}}
  //     std::get<1>(sol) è {0,1}
  //     La soluzione è: x[0] = 1.  + 0.4*x[2] - 4.3*x[3]
  //                     x[1] = 3.2 - 0.1*x[2] + 0.6*x[3]
  // Risolve il sistema a coefficienti REALI è composto dalla matrice e da
  // 'const_terms' termini noti
  template <std::floating_point X = T>
  std::tuple<typename std::vector<std::vector<X>>, std::vector<long>> solve(
      const NZVector<X>& const_terms) const;
  // Risolve il sistema a coefficienti COMPLESSI è composto dalla matrice e da
  // 'const_terms' termini noti
  template <std::floating_point X>
  std::tuple<typename std::vector<std::vector<std::complex<X>>>,
             std::vector<long>>
  solve(const NZVector<std::complex<X>>& const_terms) const;

  // Distruttore
  ~Matrix();

 private:
  std::vector<NZVector<T>> matrix_;
};

#include "../src/Matrix.inl"
#endif  // MATRIX_HPP