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
  std::clog << "\nCostruisco di default\n";
}

template <class T>
Matrix<T>::Matrix(const Matrix& that) : matrix_(that.matrix_)
{
  std::clog << "\nMatrix: Costruisco per copia\n";
}

template <class T>
Matrix<T>::Matrix(Matrix&& that) noexcept : matrix_(move(that.matrix_))
{
  std::clog << "\nMatrix: Costruisco spostando\n";
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
    throw std::ios_base::failure("File " + file_name + " could not be opened");

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
  std::clog << "\nAssegno per copia\n";
  matrix_ = that.matrix_;
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator=(Matrix&& that) noexcept
{
  std::clog << "\nAssegno spostando\n";
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
    throw std::ios_base::failure("File " + file_name + " could not be opened");
  }

  for (const NZVector<T>& nzv : *this) {
    out_file << tool::vec_to_string(nzv);
    out_file << '\n';
  }
}

template <class T>
template <std::floating_point X>
std::tuple<typename std::vector<std::vector<X>>, std::vector<long>>
Matrix<T>::solve(const NZVector<X>& const_terms) const
{
  if (this->rows() != const_terms.size())
    throw std::invalid_argument(
        "Matrix::solve: const terms vector must have as many "
        "coefficients as the rows of the matrix");
  Matrix<T> temp_mat(*this);
  NZVector<T> temp_terms(const_terms);

  std::clog << "\nTEMP_MAT:\n";
  temp_mat.print(std::clog);
  std::clog << "\nTEMP_TERMS:\n";
  temp_terms.print(std::clog);

  // Per svolgere algoritmo di Gauss devo avere la matrice in forma
  // scala-per-righe. Per ottimizzare la scelta del pivot può essere necessario
  // seguire un ordine diverso da quello in cui sono disposte le righe della
  // matrice. Quindi invece che spostare le righe, creo un elenco degli indici
  // delle righe che mano a mano entrano a far parte della struttura
  // scala-per-righe, ovvero quelle righe che contengono un pivot.
  std::vector<long> pivoted_rows;
  pivoted_rows.reserve(temp_mat.rows());

  // ALGORITMO DI GAUSS
  // ***************************************************************************
  // Massimo valore possibile del rango della matrice
  const std::size_t rank_max = std::min(temp_mat.rows(), temp_mat.cols());
  std::clog << "\nRANK_MAX = " << rank_max << '\n';

  // L'algoritmo di gauss è ripetuto finchè ci sono equazioni E incognite.
  // 'j' colonna della matrice
  for (std::size_t j{0}, delta{0}; j < rank_max + delta; ++j) {
    T pivot{0.};
    // Indice riga del pivot nella colonna j.
    long pivot_row{0};

    // Individua il pivot nella colonna j come il coefficiente a valore
    // maggiore.
    // Questo per maggiore stabilità nei risultati delle operazioni di riga,
    // dove bisogna dividere per il pivot.
    // E' necessario inizializzare this_row a -1 e incrementarlo all'inizio del
    // ciclo a causa di 'continue' contenuto nel primo 'if'.
    long this_row{-1};
    for (NZVector<T>& row : temp_mat) {
      ++this_row;
      // Se la riga 'row' fa già parte della struttura scala per righe, ovvero
      // contiene un pivot, passa a quella successiva.
      if (pivoted_rows.end() !=
          std::find(pivoted_rows.begin(), pivoted_rows.end(), this_row))
        continue;

      if (std::abs(row.at(j)) > std::abs(pivot)) {
        pivot = row.at(j);
        pivot_row = this_row;
      }
    }
    // Matrice dopo la scelta dei pivot
    std::clog << "\n[" << pivot_row << ", " << j << "] Pivot = " << pivot
              << "\nMatrix";
    std::clog << "\nMATRIX AFTER PIVOT CHOICE:\n";
    temp_mat.print(std::clog);
    std::clog << "\nTERMS AFTER PIVOT CHOICE:\n";
    temp_terms.print(std::clog);
    // Se 'pivot' è nullo, tutti i coefficienti della colonna j sono nulli,
    // ovvero la componente j del vettore soluzione è un parametro.
    // Non ci sono operazioni di riga da svolgere. Aggiorna la condizione del
    // ciclo per continuare a cercare un pivot e passa alla colonna successiva.
    // Concettualmente 'delta' permette di ignorare colonne nulle.
    // Altrimenti aggiunge l'indice riga del pivot al vettore che tiene
    // traccia della struttura scala per righe.
    if (tool::is_zero(pivot)) {
      if (this->cols() > rank_max + delta) ++delta;
      continue;
    }
    //'else' superfluo preceduto da 'continue', ma rinforza significato del
    // codice
    else
      pivoted_rows.push_back(pivot_row);

    NZVector<T>& row_pivot = temp_mat.row(pivot_row);  // Per semplicità
    // Nella colonnna j rende nulli tutti i coefficienti di righe che non
    // fanno ancora parte della struttura scala-per-righe, ovvero che non
    // contengono ancora un pivot.
    // Quindi modifica le righe di conseguenza.
    this_row = -1;
    for (NZVector<T>& row : temp_mat) {
      ++this_row;
      if (tool::is_zero(row.at(j))) continue;
      // Se la riga 'row' fa già parte della struttura scala per righe,
      // passa a quella successiva.
      if (pivoted_rows.end() !=
          std::find(pivoted_rows.begin(), pivoted_rows.end(), this_row))
        continue;
      // Variabile necessaria perchè row.at(j) cambia con le operazioni di riga
      T row_factor{row.at(j) / pivot};

      // Modifica la MATRICE
      // Rende nulli i coefficienti "sotto" pivot per realizzare la struttura
      // scala per righe
      row.set(j, 0.);
      // Sottrae alla riga 'row' la riga del pivot moltiplicata per row_factor
      for (std::size_t col(j + 1); col < row.size(); ++col) {
        row.set(col, [&](T& val) { val -= row_pivot.at(col) * row_factor; });
      }
      // Modifica il TERMINE NOTO
      // Se il sistema è NON OMOGENEO, devo eseguire le operazioni di riga
      // anche sui termini noti.
      if (const_terms.size_nz()) {
        temp_terms.set(this_row, [&](T& val) {
          val -= temp_terms.at(pivot_row) * row_factor;
        });
      }
    }
  }  // End GAUSS

  std::clog << "\nMATRIX AFTER GAUSS:\n";
  temp_mat.print(std::clog);
  std::clog << "\nTERMS AFTER GAUSS:\n";
  temp_terms.print(std::clog);
  // Ridotta la matrice alla struttura scala-per-righe, le righe dipendenti
  // sono tutte nulle.
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
        // Creo gli elementi della tuple in modo da poter utilizzare 'tie'
        return {{}, {}};
    }
  }

  // Contiene gli indici colonna delle componenti del vettore soluzione
  std::vector<long> sol_idx;
  sol_idx.reserve(pivoted_rows.size());
  // Contiene le componenti del vettore soluzione in forma di vettore.
  // Il primo elemento di ciascun vettore è il valore numerico della soluzione.
  // Se il sistema è INDETERMINATO, a ciascun valore seguono i coefficienti
  // delle componenti che hanno ruolo di parametro, inseriti seguendo l'ordine
  // decrescente degli indici colonna.
  // es. A matrice 2x4, x = (x_1, x_2, x_3, x_4), b = (5., 34.)
  //     sistema Ax = b
  //     soluzine:
  //     x_1 = 4.5  + 9.2*x_4 + 2.1*x_3 e sol_set[0] = (4.5,  9.2,  2.1)
  //     x_2 = 13.1 + 4.7*x_4 - 0.3*x_3 e sol_set[1] = (13.1, 4.7, -0.3)
  //
  std::vector<std::vector<T>> sol_set;
  sol_set.reserve(pivoted_rows.size());

  // SOLUZIONE PER SOSTITUZIONE
  // ***************************************************************************
  // risalgo la struttura scala-per-righe
  // indicates the column beyond which not search for
  // pars, i.e. inizialmente through all matrix length
  long col_pivot = static_cast<long>(temp_mat.cols());
  // Risale la struttura scala-per-righe e ottiene le soluzioni per sostituzione
  for (auto this_row = pivoted_rows.rbegin(), rend = pivoted_rows.rend();
       this_row != rend;
       ++this_row) {
    // SUBSTITUTION OF CONST TERMS, FROM OTHER UNKN
    // IF HOM, double sol(0).
    // 'temp_terms.at(*this_row)' è il termine noto della riga corrente
    T sol(temp_terms.at(*this_row));

    // Sostituire significa tenere conto dei contributi delle soluzioni delle
    // equazioni Considera i contributi alla riga corrente dei valori numerici
    // di altre soluzioni 'temp_mat.row(*this_row).at(idx)' è il coefficiente
    // nella riga corrente e nella colonna 'idx'.
    //'sol_set.at(j++).at(0)' è il valore numerico della soluzione precedente
    long j{0};
    for (long idx : sol_idx)
      sol -= temp_mat.row(*this_row).at(idx) * sol_set.at(j++).at(0);
    // 'sol' è il valore numerico della soluzione
    sol_set.push_back({sol});

    // SUBSTITUTION OF PARS

    // BACK SUB OLD PAR FIRST(i.e. from other unknowns)
    // 'col' è la pedina con cui percorro la matrice analizzandone le colonne
    // partendo dalla fine
    long col = static_cast<long>(temp_mat.cols() - 1);
    long n_par{0};
    // Verifico il indice colonna di ciascuna soluzione
    for (long pivot_idx : sol_idx) {
      // se è minore di 'col' vuol dire che ci sono dei parametri frammezzo
      for (; col > pivot_idx; --col) {
        ++n_par;
        // il valore del parametro nella riga corrente
        T par = -temp_mat.row(*this_row).at(col);

        // Porto i contributi di tutte le altre soluzioni al parametro della
        // colonna col
        j = 0;
        // 'at(temp_mat.cols()-col)' i.e. 'at(1)' first exe,
        // at(2)' second, and so on
        // Sostituisco da altre soluzioni verificando che abbiano effettivamente
        // coefficienti di quei parametri, se no out_of_range
        for (long idx : sol_idx) {
          if (col > idx)
            par -= temp_mat.row(*this_row).at(idx) * sol_set.at(j).at(n_par);
          ++j;
        }

        (*sol_set.rbegin()).push_back(par);
      }
      col = pivot_idx - 1;
    }

    // BACK SUB NEW PAR THEN
    for (long col = col_pivot - 1,
              plain_pos = temp_mat.row(*this_row).nonzero_to_plain(0);
         col > plain_pos;
         --col) {
      T new_par = temp_mat.row(*this_row).at(col);
      // se mi eseguo, at(col) c'è NUOVO parametro
      //'-' comes from equation, needed bcos coeff of unkn
      (*sol_set.rbegin()).push_back(-new_par);
    }

    // for_each (*sol_set.rbegin()) divide by
    // temp_mat.row(*this_row).plain_pos(0) (i.e. pivot)
    std::for_each((*sol_set.rbegin()).begin(),
                  (*sol_set.rbegin()).end(),
                  [&](T& val) { val /= temp_mat.row(*this_row).at_nz(0); });

    // so in next row searches pars until previous pivot
    col_pivot = temp_mat.row(*this_row).nonzero_to_plain(0);
    sol_idx.push_back(col_pivot);  // so i know which unkn is
  }

  return {sol_set, sol_idx};
}

// T = complex<X>
template <class T>
template <std::floating_point X>
std::tuple<typename std::vector<std::vector<std::complex<X>>>,
           std::vector<long>>
Matrix<T>::solve(const NZVector<std::complex<X>>& const_terms) const
{
  if (this->rows() != const_terms.size())
    throw std::invalid_argument(
        "Matrix::solve: const terms vector must have as many "
        "coefficients as the rows of the matrix");
  // Costruisce l'equivalente reale del sistema commplesso
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

  Matrix<X> temp_mat;
  temp_mat.reserve(2 * this->rows());
  // upper A and -B
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
    // Se pars=0, non si esegue.
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
  // lower B and A
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
    // Se pars=0, non si esegue.
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

  std::clog << "\nTEMP_MAT:\n";
  temp_mat.print(std::clog);
  std::clog << "\nTEMP_TERMS:\n";
  temp_terms.print(std::clog);

  std::vector<std::vector<std::complex<X>>> sol_set;
  std::vector<long> sol_idx;
  sol_set.reserve(this->rows());
  sol_idx.reserve(this->rows());

  auto tuple_sol = temp_mat.solve(temp_terms);
  std::size_t sol_length{std::get<0>(tuple_sol).size() / 2};

  // Esegui solo se il sistema ha soluzione
  if (sol_length) {
    pars = static_cast<long>((std::get<0>(tuple_sol).at(0).size() - 1) / 2);
    std::clog << "\nN_PARS = " << pars;

    std::complex<X> val;
    for (auto this_sol = std::get<0>(tuple_sol).begin(),
              end = std::get<0>(tuple_sol).begin() + sol_length;
         this_sol != end;
         ++this_sol) {
      val.imag(this_sol->at(0));
      val.real((this_sol + sol_length)->at(0));
      sol_set.push_back({val});

      std::complex<X> par_coeff;
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
  std::clog << "\nMatrix: Distruggo\n";
}