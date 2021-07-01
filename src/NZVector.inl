// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
//
// Si fa riferimento a indici e posizioni, che partono da 0, e a distanze e
// lunghezze, che partono da 1.
// Si parla di elenco dei VALORI riferendosi all'attributo 'val_', elenco
// degli INDICI riferendosi all'attributo 'idx_', e elenco ESTESO riferendosi
// alla forma estesa del vettore in cui vengono elencati anche i valori nulli.
// es. elenco esteso       = (1,0,0,4,9,0,0,0,2,0,0)
//     elnco dei valori    = (1,4,9,2) = val_
//     elenco degli indici = (0,3,4,8,10) = idx_
// L'elenco degli indici termina con un indice di controllo pari alla lunghezza
// dell'elenco esteso.
//
#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include "../inc/NZVector.hpp"
#include "../inc/tool.hpp"

template <class T>
NZVector<T>::NZVector()
{
  // std::clog << "\nCostruisco di default\n";
}

template <class T>
NZVector<T>::NZVector(const NZVector& that) : idx_(that.idx_), val_(that.val_)
{
  // std::clog << "\nNZV: Costruisco per copia\n";
}

template <class T>
NZVector<T>::NZVector(NZVector&& that) noexcept
    : idx_(move(that.idx_)), val_(move(that.val_))
{
  // std::clog << "\nNZV: Costruisco spostando\n";
  // Class invariant: l'elenco degli indici deve contenere almeno l'indice di
  // controllo
  that.idx_.assign({0});
}

template <class T>
NZVector<T>::NZVector(const std::initializer_list<T>& list)
{
  this->reserve(list.size());
  for (const T& val : list) this->push_back(val);
}

template <class T>
NZVector<T>::NZVector(std::size_t new_cap)
{
  val_.reserve(new_cap);
  idx_.reserve(new_cap + 1);
}

template <class T>
NZVector<T>::NZVector(std::string const& file_name)
{
  std::ifstream in_file(file_name);
  if (!in_file)
    throw std::ios_base::failure("Non è stato possibile aprire il file " +
                                 file_name);

  std::string str_line;
  while (std::getline(in_file, str_line)) {
    if (not str_line.length()) continue;  // ignoro righe vuote
    tool::string_to_vec(str_line, *this);
  }
}

template <class T>
NZVector<T>& NZVector<T>::operator=(const NZVector& that)
{
  std::clog << "\nAssegno per copia\n";
  idx_ = that.idx_;
  val_ = that.val_;
  return *this;
}

template <class T>
NZVector<T>& NZVector<T>::operator=(NZVector&& that) noexcept
{
  std::clog << "\nAssegno spostando\n";
  idx_ = move(that.idx_);
  val_ = move(that.val_);
  // Class invariant: l'elenco degli indici deve contenere almeno l'indice di
  // controllo
  that.idx_.assign({0});
  return *this;
}

template <class T>
NZVector<T>& NZVector<T>::operator=(const std::initializer_list<T>& list)
{
  this->clear();
  this->reserve(list.size());
  for (const T& val : list) this->push_back(val);
  return *this;
}

template <class T>
void NZVector<T>::reserve(const std::size_t new_cap)
{
  if (this->capacity_nz() > new_cap) {
    this->val_.reserve(new_cap);
    // idx_ contiene un indice di controllo
    this->idx_.reserve(new_cap + 1);
  }
}

template <class T>
void NZVector<T>::set(const std::size_t pos, const T& val)
{
  if (pos >= this->size())
    throw std::out_of_range("NZVector::set: l'indice " + std::to_string(pos) +
                            " non corrisponde a nessun coefficiente.");

  const long pos_nonzero{this->plain_to_nonzero(pos)};

  if (pos_nonzero == -1) {
    if (not tool::is_zero(val)) {
      long pos_insert{0};
      // Devo inserire 'pos' prima degli indici di cui è minore e dopo quelli
      // di cui è maggiore.
      // Alla stessa posizione inserisco 'val'.
      for (std::size_t idx : idx_) {
        if (pos < idx) break;
        ++pos_insert;
      }
      idx_.insert(idx_.cbegin() + pos_insert, pos);
      val_.insert(val_.cbegin() + pos_insert, val);
    } else  // sostituisco 0 con 0
      ;

  } else if (tool::is_zero(val)) {
    // Elimino dal vettore il valore non-zero del coefficiente e il suo
    // indice
    idx_.erase(idx_.cbegin() + pos_nonzero);
    val_.erase(val_.cbegin() + pos_nonzero);
  } else
    *(val_.begin() + pos_nonzero) = val;  // Cambio valore del coefficiente
}

// 'action' deve prendere un solo argomento per riferimento, altrimenti il
//  valore del coefficiente che si vuole modificare diventa 0.
template <class T>
template <std::invocable<T&> UnaryFunction>
void NZVector<T>::set(const std::size_t pos, UnaryFunction action)
{
  if (pos >= this->size())
    throw std::out_of_range("NZVector::set: l'indice " + std::to_string(pos) +
                            " non corrisponde a nessun coefficiente.");

  T val{0.};
  const long pos_nonzero{this->plain_to_nonzero(pos)};

  if (pos_nonzero == -1) {  // pos non è presente nell'elenco degli indici
                            // ovvero corrisponde a un valore zero
    action(val);

    if (not tool::is_zero(val)) {  // 'val' diverso da zero
      long pos_insert = 0;
      // Devo inserire 'pos' prima degli indici di cui è minore e dopo quelli
      // di cui è maggiore.
      // Alla stessa posizione inserisco 'val'.
      for (std::size_t idx : idx_) {
        if (idx > pos) break;
        ++pos_insert;
      }
      idx_.insert(idx_.begin() + pos_insert, pos);
      val_.insert(val_.begin() + pos_insert, val);
    } else  // sostituisco 0 con 0
      ;
  } else {  // 'pos' è presente nell'elenco degli indici, ovvero corrisponde a
            // un valore non nullo
    val = *(val_.begin() + pos_nonzero);
    action(val);

    if (tool::is_zero(val)) {
      // Elimino dal vettore il valore non-zero del coefficiente e il suo
      // indice
      idx_.erase(idx_.cbegin() + pos_nonzero);
      val_.erase(val_.cbegin() + pos_nonzero);
    } else
      *(val_.begin() + pos_nonzero) = val;
  }
}

template <class T>
void NZVector<T>::push_back(const T& value)
{
  if (not tool::is_zero(value)) {
    // l'indice di controllo procede
    idx_.push_back(*idx_.rbegin());
    val_.push_back(value);
  }
  // incrementa il valore dell'indice di controllo
  ++(*idx_.rbegin());
}

template <class T>
void NZVector<T>::clear()
{
  this->val_.clear();
  // idx_ contiene un indice di controllo
  this->idx_.assign({0});
}

template <class T>
T NZVector<T>::at(const std::size_t pos) const
{
  if (pos >= this->size())
    throw std::out_of_range("NZVector::set: l'indice " + std::to_string(pos) +
                            " non corrisponde a nessun coefficiente.");

  const long pos_nonzero{this->plain_to_nonzero(pos)};
  // 'pos' non è presente nell'elenco degli indici, ovvero il valore è nullo
  if (pos_nonzero == -1) return 0.;

  return val_.at(pos_nonzero);
};

template <class T>
T NZVector<T>::at_nz(const std::size_t pos_nz) const
{
  return val_.at(pos_nz);
}

// L'ultimo elemento dell'elenco degli indici è l'indice di controllo.
// Non corrisponde a nessun valore ed è pari alla lunghezza del vettore esteso.
template <class T>
std::size_t NZVector<T>::size() const
{
  return *idx_.rbegin();
}

template <class T>
std::size_t NZVector<T>::size_nz() const
{
  return val_.size();
}

template <class T>
std::size_t NZVector<T>::max_size_nz() const
{
  return val_.max_size();
}

template <class T>
std::size_t NZVector<T>::capacity_nz() const
{
  return val_.capacity();
}

template <class T>
long NZVector<T>::plain_to_nonzero(const size_t pos) const
{
  if (pos >= this->size())
    throw std::out_of_range("NZVector::set: l'indice " + std::to_string(pos) +
                            " non corrisponde a nessun coefficiente.");

  // Cerco 'pos' nell'elenco degli indici 'idx_'.
  // A causa dello schema di storage, nel vettore 'idx_' un indice qualsiasi
  // 'pos' può trovarsi nel range [0,pos] oppure non trovarsi affatto.
  // Se 'pos' è minore della lunghezza dell'elenco degli indici,
  // ALLORA posso escludere gli indici oltre 'pos' dalla mia ricerca.
  // e l'indice avrà più probabilità di essere trovato vicino alla posizione
  // 'pos' che occurebbe nell'elenco esteso, piuttosto che all'inizio di idx_.
  // Questo giustifica l'uso degli iteratori inversi.
  // ALTRIMENTI sono costretto a cercare lungo tutto l'elenco degli indici.
  // '-1' perché 'crend' punta la posizione precedente la prima e il valore
  // zero di 'pos' identifica la prima posizione.
  std::reverse_iterator<std::vector<long>::const_iterator> reverse_it;
  if (pos < idx_.size())
    reverse_it = std::find(idx_.crend() - 1 - pos, idx_.crend(), pos);
  else
    reverse_it = std::find(idx_.crbegin(), idx_.crend(), pos);

  // La distanza tra il primo elemento e crend() è 1, ma la poszione del primo
  // elemento è 0.
  return std::distance(reverse_it, idx_.crend()) - 1;
}

template <class T>
long NZVector<T>::nonzero_to_plain(const std::size_t pos_nz) const
{
  return idx_.at(pos_nz);
}

template <class T>
void NZVector<T>::print(std::ostream& out) const
{
  long i{0};
  out << "\nidx_:\n{";
  for (const long& idx : idx_) out << "\n  [" << i++ << "] = " << idx;
  out << "\n}";

  i = 0;
  out << "\nval_:\n{";
  for (const T& val : val_) out << "\n  [" << i++ << "] = " << val;
  out << "\n}";
}

template <class T>
NZVector<T>::~NZVector()
{
  // std::clog << "\nNZV: Distruggo\n";
}
