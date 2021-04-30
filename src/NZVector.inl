#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include "../inc/NZVector.hpp"
#include "../inc/tool.hpp"

template <class T>
NZVector<T>::NZVector()
{
  clog << "\nCostruisco di default\n";
}

template <class T>
NZVector<T>::NZVector(const NZVector& that) : idx_(that.idx_), val_(that.val_)
{
  clog << "\nCostruisco per copia\n";
}

template <class T>
NZVector<T>::NZVector(NZVector&& that)
    : idx_(move(that.idx_)), val_(move(that.val_))
{
  clog << "\nCostruisco spostando\n";
  // L'elenco degli indici deve contenere almeno l'indice di controllo
  that.idx_.assign({0});
}

template <class T>
NZVector<T>& NZVector<T>::operator=(const NZVector& that)
{
  clog << "\nAssegno per copia\n";

  idx_ = that.idx_;
  val_ = that.val_;
  return *this;
}

template <class T>
NZVector<T>& NZVector<T>::operator=(NZVector&& that)
{
  clog << "\nAssegno spostando\n";
  idx_ = move(that.idx_);
  val_ = move(that.val_);
  // L'elenco degli indici deve contenere almeno l'indice di controllo
  that.idx_.assign({0});
  return *this;
}

template <class T>
T NZVector<T>::at(const size_t pos) const
{
  // 'pos' è negativo o maggiore uguale dell'indice di controllo
  if (pos >= this->size())
    throw out_of_range("NZVector::at: index can't be greater than size");

  reverse_iterator<vector<long>::const_iterator> pos_it;
  // Cerco 'pos' nell'elenco degli indici 'idx_'.
  // A causa dello schema di storage, nel vettore 'idx_' un indice qualsiasi
  // 'pos' può trovarsi nel range [0,pos] oppure non trovarsi affatto.
  // Se 'pos' è minore della lunghezza dell'elenco degli indici,
  // ALLORA posso escludere gli indici oltre 'pos' dalla mia ricerca.
  // ALTRIMENTI sono costretto a cercare lungo tutto l'elenco degli indici.
  // '+1' perchè pos è un indice, size è una dimensione
  if (pos + 1 < idx_.size())
    // '-1' perché 'crend' punta la posizione precedente la prima e il valore
    // zero di 'pos' identifica la prima posizione.
    pos_it = find(idx_.crend() - pos - 1, idx_.crend(), pos);
  else
    pos_it = find(idx_.crbegin(), idx_.crend(), pos);

  // 'pos' non è presente nell'elenco degli indici, ovvero il valore è nullo
  if (idx_.crend() == pos_it) return 0.;

  // 'pos' è presente nell'elenco degli indici, ed è puntato dall'iteratore
  // 'pos_it'.
  // '-1' perché per la prima posizione 'distance' vale 1 e 'nz_pos' vale 0.
  long pos_nonzero = distance(pos_it, idx_.crend()) - 1;
  return val_.at(pos_nonzero);  // throw out_of_range
};

// L'ultimo elemento dell'elenco degli indici è l'indice di controllo.
// Non corrisponde a nessun valore ed è pari alla lunghezza del vettore esteso.
template <class T>
size_t NZVector<T>::size() const
{
  return *idx_.rbegin();
}

template <class T>
void NZVector<T>::print(ostream& out) const
{
  long i{0};
  cout << "\nidx_:\n{";
  for (const auto& idx : idx_) out << "\n  [" << i++ << "] = " << idx;
  cout << "\n}";

  i = 0;
  cout << "\nval_:\n{";
  for (const auto& val : val_) out << "\n  [" << i++ << "] = " << val;
  cout << "\n}";
}

template <class T>
void NZVector<T>::print_plain(ostream&) const
{
}

template <class T>
void NZVector<T>::push_back(const T& value)
{
  if (!tool::is_zero(value)) {
    // l'indice di controllo procede
    idx_.push_back(*idx_.rbegin());
    val_.push_back(value);
  }
  // incrementa il valore dell'indice di controllo
  ++(*idx_.rbegin());
}

template <class T>
NZVector<T>::~NZVector()
{
  clog << "\nDistruggo\n";
}