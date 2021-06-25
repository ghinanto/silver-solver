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
  Matrix(std::ifstream&);
  Matrix(std::string const&);
  // costruisce una matrice a valori REALI generati random nell'intervallo
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

  void reserve(std::size_t);
  void push_back(NZVector<T> const&);
  void push_back(NZVector<T>&&);
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
  void to_file(std::string const&) const;
  void to_file(std::ofstream&) const;

  // Riempie il vettore sol con le componenti del vettore soluzione del sistema
  // a coefficienti REALI composto dalla matrice e da 'const_terms' termini noti
  template <std::floating_point X = T>
  std::tuple<typename std::vector<std::vector<X>>, std::vector<long>> solve(
      const NZVector<X>& const_terms) const;
  // Riempie il vettore sol con le componenti del vettore soluzione del sistema
  // a coefficienti COMPLESSI composto dalla matrice e da 'const_terms' termini
  // noti
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