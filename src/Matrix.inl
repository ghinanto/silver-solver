// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
#include <algorithm>
#include <cmath>
#include <complex>
#include <exception>
#include <iostream>
#include <tuple>
#include <vector>
#include "../inc/Matrix.hpp"

template <class T>
Matrix<T>::Matrix()
{
  // std::clog << "\nCostruisco di default\n";
}

template <class T>
Matrix<T>::Matrix(const Matrix& that) : matrix_(that.matrix_)
{
  // std::clog << "\nMatrix: Costruisco per copia\n";
}

template <class T>
Matrix<T>::Matrix(Matrix&& that) noexcept : matrix_(move(that.matrix_))
{
  // std::clog << "\nMatrix: Costruisco spostando\n";
}

template <class T>
Matrix<T>::Matrix(std::ifstream& in_file)
{
  std::string str_line;
  while (std::getline(in_file, str_line)) {
    if (not str_line.length()) continue;
    NZVector<T> vec_line;
    tool::string_to_vec(str_line, vec_line);
    matrix_.push_back(vec_line);
  }
}

template <class T>
Matrix<T>::Matrix(std::string const& file_name)
{
  std::ifstream in_file(file_name, std::ios_base::in);
  if (!in_file)
    throw std::ios_base::failure("Non è stato possibile aprire il file " +
                                 file_name);

  std::string str_line;
  while (std::getline(in_file, str_line)) {
    if (not str_line.length()) continue;
    NZVector<T> vec_line;
    tool::string_to_vec(str_line, vec_line);
    matrix_.push_back(vec_line);
  }
}

template <class T>
Matrix<T>::Matrix(std::size_t rows,
                  std::size_t cols,
                  T first_bound,
                  T second_bound)
{
  // this->reserve(rows);
  for (std::size_t i{0}; i < rows; ++i) {
    this->emplace_back(tool::rand_to_vec(cols, first_bound, second_bound));
  }
}

template <class T>
template <class Arithmetic>
Matrix<T>::Matrix(std::size_t rows,
                  std::size_t cols,
                  short complex_on_tot,
                  Arithmetic first_bound,
                  Arithmetic second_bound)
{
  this->reserve(rows);
  for (std::size_t i{0}; i < rows; ++i) {
    this->emplace_back(
        tool::rand_to_vec(cols, complex_on_tot, first_bound, second_bound));
  }
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& that)
{
  // std::clog << "\nAssegno per copia\n";
  matrix_ = that.matrix_;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator=(Matrix&& that) noexcept
{
  // std::clog << "\nAssegno spostando\n";
  matrix_ = move(that.matrix_);
  return *this;
}

template <class T>
std::vector<NZVector<T>>::iterator Matrix<T>::begin()
{
  return matrix_.begin();
}

template <class T>
std::vector<NZVector<T>>::const_iterator Matrix<T>::begin() const
{
  return matrix_.begin();
}

template <class T>
std::vector<NZVector<T>>::iterator Matrix<T>::end()
{
  return matrix_.end();
}

template <class T>
std::vector<NZVector<T>>::const_iterator Matrix<T>::end() const
{
  return matrix_.end();
}

template <class T>
std::reverse_iterator<typename std::vector<NZVector<T>>::iterator>
Matrix<T>::rbegin()
{
  return matrix_.rbegin();
}

template <class T>
std::reverse_iterator<typename std::vector<NZVector<T>>::const_iterator>
Matrix<T>::rbegin() const
{
  return matrix_.rbegin();
}

template <class T>
std::reverse_iterator<typename std::vector<NZVector<T>>::iterator>
Matrix<T>::rend()
{
  return matrix_.rend();
}

template <class T>
std::reverse_iterator<typename std::vector<NZVector<T>>::const_iterator>
Matrix<T>::rend() const
{
  return matrix_.rend();
}

template <class T>
void Matrix<T>::reserve(std::size_t new_cap)
{
  return matrix_.reserve(new_cap);
}

template <class T>
void Matrix<T>::push_back(NZVector<T> const& vec)
{
  return matrix_.push_back(vec);
}

template <class T>
void Matrix<T>::push_back(NZVector<T>&& vec)
{
  return matrix_.push_back(vec);
}

template <class T>
template <class... Args>
std::vector<NZVector<T>>::reference Matrix<T>::emplace_back(Args&&... args)
{
  return matrix_.emplace_back(std::move(args...));
}

template <class T>
NZVector<T>& Matrix<T>::row(std::size_t pos)
{
  return matrix_.at(pos);
}

template <class T>
void Matrix<T>::clear()
{
  return matrix_.clear();
}

template <class T>
std::size_t Matrix<T>::rows() const
{
  return matrix_.size();
}

template <class T>
std::size_t Matrix<T>::cols() const
{
  return matrix_.at(0).size();
}

template <class T>
void Matrix<T>::print(std::ostream& out) const
{
  long i{0};
  for (const NZVector<T>& row : *this) {
    out << "\n\nrow[" << i++ << "]\n";
    row.print(out);
  }
}

template <class T>
void Matrix<T>::to_file(std::string const& file_name) const
{
  std::ofstream out_file(file_name, std::ios::out);

  if (!out_file) {  // exit program if ifstream could not open file
    throw std::ios_base::failure("Non è stato possibile aprire il file " +
                                 file_name);
  }

  for (const NZVector<T>& nzv : *this) {
    out_file << tool::vec_to_string(nzv);
    out_file << '\n';
  }
}

// Il sistema viene risolto tramite algoritmo di Gauss e sostituzione.
// L'algoritmo di Gauss riduce la matrice in forma scala per righe, eseguendo
// operazioni di riga, in questo modo:
// (1)  cerca in una colonna l'elemento maggiore in valore assoluto (pivot)
// (2)  tramite operazioni di riga rende nulli gli altri coefficienti della
//      colonna che non facciano parte di righe già contenenti un pivot
// (3)  passa alla colonna successiva
template <class T>
template <std::floating_point X>
std::tuple<typename std::vector<std::vector<X>>, std::vector<long>>
Matrix<T>::solve(const NZVector<X>& const_terms) const
{
  if (this->rows() != const_terms.size())
    throw std::invalid_argument(
        "Matrix::solve: Il numero di termini noti è diverso dal numero di "
        "equazioni");
  Matrix<T> temp_mat(*this);
  NZVector<T> temp_terms(const_terms);
  // Crea un elenco degli indici delle righe che mano a mano entrano a far
  // parte della struttura scala-per-righe, ovvero quelle righe che contengono
  // un pivot.
  std::vector<long> pivoted_rows;
  pivoted_rows.reserve(temp_mat.rows());

  // ALGORITMO DI GAUSS
  // ***************************************************************************
  const std::size_t rank_max = std::min(temp_mat.rows(), temp_mat.cols());

  // L'algoritmo di gauss è ripetuto finchè ci sono equazioni E incognite.
  for (std::size_t this_col{0}, delta{0}; this_col < rank_max + delta;
       ++this_col) {
    T pivot{0.};
    // Indice riga del pivot nella colonna this_col.
    long pivot_row{0};

    // Individua il pivot nella colonna this_col come il coefficiente a valore
    // maggiore.
    // Questo per maggiore stabilità nei risultati delle operazioni di riga in
    // cui bisogna dividere per il pivot.
    // E' necessario inizializzare this_row a '-1' e incrementarlo all'inizio
    // del ciclo a causa di 'continue' contenuto nel primo 'if'.
    long this_row{-1};
    for (NZVector<T>& row : temp_mat) {
      ++this_row;
      // Se la riga 'row' fa già parte della struttura scala per righe, ovvero
      // contiene un pivot, passa a quella successiva.
      if (pivoted_rows.end() !=
          std::find(pivoted_rows.begin(), pivoted_rows.end(), this_row))
        continue;

      if (std::abs(row.at(this_col)) > std::abs(pivot)) {
        pivot = row.at(this_col);
        pivot_row = this_row;
      }
    }

    // Se qui 'pivot' è nullo, tutti i coefficienti della colonna this_col sono
    // nulli, ovvero la componente this_col del vettore soluzione è un
    // parametro. Non ci sono operazioni di riga da svolgere. Passa alla
    // colonna successiva. Inoltre poichè una variabile interna assume il ruolo
    // di parametro, per completare la forma scala per righe è necessario
    // aumentare il limite del ciclo incrementando 'delta'. Concettualmente
    // 'delta' permette di ignorare colonne nulle.
    if (tool::is_zero(pivot)) {
      if (this->cols() > rank_max + delta) ++delta;
      continue;
    }
    //'else' superfluo preceduto da 'continue', ma rinforza significato del
    // codice
    else
      pivoted_rows.push_back(pivot_row);

    // Nella colonnna this_col rende nulli tutti i coefficienti di righe che non
    // fanno ancora parte della struttura scala-per-righe, ovvero che non
    // contengono ancora un pivot.
    // Quindi modifica le righe di conseguenza.
    NZVector<T>& row_pivot = temp_mat.row(pivot_row);  // Per semplicità
    this_row = -1;
    for (NZVector<T>& row : temp_mat) {
      ++this_row;
      if (tool::is_zero(row.at(this_col))) continue;
      // Se la riga 'row' fa già parte della struttura scala per righe,
      // passa a quella successiva.
      if (pivoted_rows.end() !=
          std::find(pivoted_rows.begin(), pivoted_rows.end(), this_row))
        continue;
      // Variabile necessaria perchè row.at(this_col) cambia con le operazioni
      // di riga
      T row_factor{row.at(this_col) / pivot};

      // Modifica la MATRICE
      // Rende nulli i coefficienti "sotto" il pivot per realizzare la struttura
      // scala per righe
      row.set(this_col, 0.);
      // Sottrae alla riga 'row' la riga del pivot moltiplicata per row_factor
      for (std::size_t col(this_col + 1); col < row.size(); ++col)
        row.set(col, [&](T& val) { val -= row_pivot.at(col) * row_factor; });

      // Modifica il TERMINE NOTO
      // Se il sistema è NON OMOGENEO, devo eseguire le operazioni di riga
      // anche sui termini noti.
      if (const_terms.size_nz())
        temp_terms.set(this_row, [&](T& val) {
          val -= temp_terms.at(pivot_row) * row_factor;
        });
    }
  }  // End GAUSS

  // Ridotta la matrice alla struttura scala-per-righe, le righe linearmente
  // dipendenti sono nulle.
  // Se il sistema è NON omogeneo, queste righe potrebbero essere
  // inconsistenti, ovvero riga della matrice nulla e termine noto non nullo.
  // In questo caso il sistema è impossibile.
  if (const_terms.size_nz()) {
    for (std::size_t this_row{0}, rows{temp_mat.rows()}; this_row < rows;
         ++this_row) {
      if (pivoted_rows.end() !=
          std::find(pivoted_rows.begin(), pivoted_rows.end(), this_row))
        continue;

      if (not tool::is_zero(temp_terms.at(this_row)))
        // Creo gli elementi della tuple in modo da poter utilizzare std::tie
        return {{}, {}};
    }
  }

  // Contiene gli indici colonna delle componenti del vettore soluzione
  std::vector<long> sol_idx;
  sol_idx.reserve(pivoted_rows.size());
  // Contiene le componenti del vettore soluzione
  std::vector<std::vector<T>> sol_set;
  sol_set.reserve(pivoted_rows.size());

  // SOLUZIONE PER SOSTITUZIONE
  // ***************************************************************************
  //
  long previous_pivot_idx = static_cast<long>(temp_mat.cols());
  // Risale la struttura scala-per-righe e ottiene le soluzioni per sostituzione
  for (auto this_row = pivoted_rows.rbegin(), rend = pivoted_rows.rend();
       this_row != rend;
       ++this_row) {
    // Il valore numerico della soluzione
    // 'temp_terms.at(*this_row)' è il termine noto della riga corrente
    T sol(temp_terms.at(*this_row));
    // Considera i contributi alla riga corrente dei valori numerici di altre
    // soluzioni
    //'temp_mat.row(*this_row).at(idx)' è il coefficiente di x[idx] nella riga
    // corrente
    //'sol_set.at(j++).at(0)' è il valore numerico della soluzione precedente
    long j{0};
    for (long idx : sol_idx)
      sol -= temp_mat.row(*this_row).at(idx) * sol_set.at(j++).at(0);
    sol_set.push_back({sol});

    // 'col' è l'indice con cui percorro la matrice analizzandone le colonne
    // partendo dalla fine
    long col = static_cast<long>(temp_mat.cols() - 1);
    // Numero di parametri
    long n_par{0};

    // Sostituisco coefficienti di parametri già contenuti nelle soluzioni
    // precedenti.
    // 'sol_idx' contiene gli indici colonna delle soluzioni in ordine
    //  decrescente. Quindi se col > pivot_idx, significa che gli indici che li
    //  separano sono quelli di parametri.
    for (long pivot_idx : sol_idx) {
      for (; col > pivot_idx; --col) {
        ++n_par;
        // coefficiente del parametro nella riga corrente
        T par = -temp_mat.row(*this_row).at(col);

        // Sostituisco da altre soluzioni verificando che abbiano effettivamente
        // coefficienti di quei parametri, altrimenti 'sol_set.at(j).at(n_par)'
        // non esiste
        // 'temp_mat.row(*this_row).at(idx)' è il coefficiente di x[idx] nella
        // riga corrente
        // 'sol_set.at(j).at(n_par)' è il coefficiente del parametro in x[idx]
        j = 0;
        for (long idx : sol_idx) {
          // x[idx] può contenere solo paramteri di indice maggiore del proprio
          if (idx < col)
            par -= temp_mat.row(*this_row).at(idx) * sol_set.at(j).at(n_par);
          ++j;
        }

        (*sol_set.rbegin()).push_back(par);
      }
      // Leggo la matrice a partire dalla colonna precedente quella corrente,
      // che so già contenere un pivot
      col = pivot_idx - 1;
    }

    // Considero nuovi parametri introdotti dalla riga corrente
    const long this_pivot_idx = temp_mat.row(*this_row).nonzero_to_plain(0);
    for (long col = previous_pivot_idx - 1; col > this_pivot_idx; --col) {
      T new_par = temp_mat.row(*this_row).at(col);
      (*sol_set.rbegin()).push_back(-new_par);
    }

    // Divide tutto la componente corrente del vettore soluzione per il pivot
    // 'temp_mat.row(*this_row).at_nz(0)' è il pivot
    std::for_each((*sol_set.rbegin()).begin(),
                  (*sol_set.rbegin()).end(),
                  [&](T& val) { val /= temp_mat.row(*this_row).at_nz(0); });

    sol_idx.push_back(this_pivot_idx);  // so i know which unkn is
    // so in next row searches pars until previous pivot
    previous_pivot_idx = this_pivot_idx;
  }

  return {sol_set, sol_idx};
}

// T = complex<X>
// Risolve un sistema a coefficienti complessi risolvendo il sistema
// equivalente reale.
// Dato un sistema Mx=c con M=A+i*B matrice, x=y+i*z, c=r+i*s vettore, il
// sistema equivalente reale è dato da:
//   | A -B | |x| = |p|
//   | B  A | |y|   |q|
//
// Nel caso di sistemi indeterminati il sistema equivalente viene ottenuto
// spostando le colonne che si riferiscono alle parti reali e immaginarie dei
// parametri in fondo alla matrice, ovvero:
//   | A(x) -B(x) | A(p) -B(p) | |x| = |r|
//   | B(x)  A(x) | B(p)  A(p) | |y|   |s|
// dove A(x), B(x) contengono le parti reali e immaginarie delle incognite,
// A(p) e B(p) le parti reali e immaginarie dei parametri.
template <class T>
template <std::floating_point X>
std::tuple<typename std::vector<std::vector<std::complex<X>>>,
           std::vector<long>>
Matrix<T>::solve(const NZVector<std::complex<X>>& const_terms) const
{
  if (this->rows() != const_terms.size())
    throw std::invalid_argument(
        "Matrix::solve: Il numero di termini noti è diverso dal numero di "
        "equazioni");
  // Costruisce l'equivalente reale del vettore dei termini noti
  NZVector<X> temp_terms;
  temp_terms.reserve(2 * const_terms.size_nz());
  for (std::size_t i{0}, length{const_terms.size()}; i < length; ++i) {
    temp_terms.push_back(const_terms.at(i).real());
  }
  for (std::size_t i{0}, length{const_terms.size()}; i < length; ++i) {
    temp_terms.push_back(const_terms.at(i).imag());
  }

  long pars = static_cast<long>(this->cols() - this->rows());
  if (pars < 0) pars = 0;
  // Costruisce l'equivalente reeale della matrice
  Matrix<X> temp_mat;
  temp_mat.reserve(2 * this->rows());
  // Scorre le righe della matrice
  for (const NZVector<std::complex<X>>& this_row : *this) {
    temp_mat.emplace_back(2 * this_row.size_nz());

    // Riempie A(x) e -B(x)
    for (std::size_t col{0}, length{this_row.size() - pars}; col < length;
         ++col) {
      temp_mat.rbegin()->push_back(this_row.at(col).real());
    }
    for (std::size_t col{0}, length{this_row.size() - pars}; col < length;
         ++col) {
      temp_mat.rbegin()->push_back(-this_row.at(col).imag());
    }

    // Riempie A(p) e -B(p)
    // Se pars==0, non si esegue.
    for (std::size_t col{this_row.size() - pars}, length{this_row.size()};
         col < length;
         ++col) {
      temp_mat.rbegin()->push_back(this_row.at(col).real());
    }
    for (std::size_t col{this_row.size() - pars}, length{this_row.size()};
         col < length;
         ++col) {
      temp_mat.rbegin()->push_back(-this_row.at(col).imag());
    }
  }
  // Scorre le righe della matrice
  for (const NZVector<std::complex<X>>& this_row : *this) {
    temp_mat.emplace_back(2 * this_row.size_nz());

    // Riempie B(x) e A(x)
    for (std::size_t col{0}, length{this_row.size() - pars}; col < length;
         ++col) {
      temp_mat.rbegin()->push_back(this_row.at(col).imag());
    }
    for (std::size_t col{0}, length{this_row.size() - pars}; col < length;
         ++col) {
      temp_mat.rbegin()->push_back(this_row.at(col).real());
    }

    // Riempie B(p) e A(p)
    // Se pars==0, non si esegue.
    for (std::size_t col{this_row.size() - pars}, length{this_row.size()};
         col < length;
         ++col) {
      temp_mat.rbegin()->push_back(this_row.at(col).imag());
    }
    for (std::size_t col{this_row.size() - pars}, length{this_row.size()};
         col < length;
         ++col) {
      temp_mat.rbegin()->push_back(this_row.at(col).real());
    }
  }

  std::vector<std::vector<std::complex<X>>> sol_set;
  std::vector<long> sol_idx;
  sol_set.reserve(this->rows());
  sol_idx.reserve(this->rows());

  auto tuple_sol = temp_mat.solve(temp_terms);
  // Dimensione della soluzione.
  // 'std::get<0>(tuple_sol)' è la soluzione reale equivalente, che è lunga il
  // doppio
  std::size_t sol_length{std::get<0>(tuple_sol).size() / 2};

  // Scrive la soluzione usando i numeri complessi a partire dalla soluzione
  // reale equivalente ottenuta
  if (sol_length) {  // Esegui solo se il sistema ha soluzione
    pars = static_cast<long>((std::get<0>(tuple_sol).at(0).size() - 1) / 2);

    std::complex<X> val;  // Valore numerico
    for (auto this_sol = std::get<0>(tuple_sol).begin(),
              end = std::get<0>(tuple_sol).begin() + sol_length;
         this_sol != end;
         ++this_sol) {
      val.imag(this_sol->at(0));
      val.real((this_sol + sol_length)->at(0));
      sol_set.push_back({val});

      std::complex<X> par_coeff;  // Coeffiente del parametro
      for (auto par_coeff_it = this_sol->rbegin(),
                end = this_sol->rbegin() + pars;
           par_coeff_it != end;
           ++par_coeff_it) {
        par_coeff.imag(*par_coeff_it);
        par_coeff.real(*(par_coeff_it + pars));
        sol_set.rbegin()->push_back(par_coeff);
      }

      // '-1' converte distanza in indice
      long idx{std::distance(this_sol, end) - 1};
      sol_idx.push_back(idx);
    }
  }
  return {sol_set, sol_idx};
}

template <class T>
Matrix<T>::~Matrix()
{
  // std::clog << "\nMatrix: Distruggo\n";
}