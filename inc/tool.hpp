// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
//
// Contiene funzioni di varia utilità in particolare nell'ambito della
// conversione di NZVector in stringhe o file e viceversa.
#ifndef TOOL_H
#define TOOL_H

#include <complex>
#include <iostream>
#include <string>

namespace tool {

// Ripete l'input di un valore per la variabile fino a che riesce correttamente.
// Interagisce con l'utente tramite l'output.
template <class T>
void get_input(T& variable,
               std::istream& = std::cin,
               std::ostream& = std::cout);

// Restituisce 'true' se l'argomento ha valore zero.
// Nel caso di variabili numeriche a valori decimali viene considerato nullo
// solo un valore maggiore del minimo rappresentabile dall'architettura.
template <std::floating_point T>
bool is_zero(const T&, const T& precision = std::numeric_limits<T>::epsilon());

template <std::floating_point T>
bool is_zero(const std::complex<T>&,
             const T& precision = std::numeric_limits<T>::epsilon());

template <std::integral T>
bool is_zero(const T&);

template <std::integral T>
bool is_zero(const std::complex<T>&);

// Restituisce un vettore con 'size' valori random reali
// Quando usato per inizializzare un NZVector, il compilatore elide la copia
// (NRVO) costruendo direttamente il vettore di destinazione.
template <std::floating_point T>
NZVector<T> rand_to_vec(std::size_t size, T first_bound, T second_bound = 0.);

// Restituisce un vettore con 'size' valori random complessi.
// 'complex_on_tot' varia da [0,100] e rappresenta la percentuale di
// coefficienti con parte immaginaria diversa da zero.
// Elimina il contenuto precedente del vettore.
// Quando usato per inizializzare un NZVector, il compilatore elide la copia
// (NRVO) costruendo direttamente il vettore di destinazione.
template <std::floating_point T>
NZVector<std::complex<T>> rand_to_vec(std::size_t size,
                                      short complex_on_tot,
                                      T first_bound,
                                      T second_bound = 0.);

// Scrive un vettore su file, utilizzando la modalità di apertura indicata.
// Nel caso il file esista già, std::ios::out sovrascrive il contenuto,
// std::ios::app scrive di seguito al contenuto del file.
// I coefficienti sono scritti in notazione scientifica tenendo 4 cifre decimali
template <class T>
void vec_to_file(const NZVector<T>&,
                 const std::string& file_name,
                 const std::ios_base::openmode = std::ios::out);

template <class T>
void vec_to_file(const NZVector<T>&, std::ofstream&);

// Scrive il vettore su uno string stream.
// I coefficienti sono scritti in notazione scientifica tenendo 4 cifre decimali
template <class T>
void vec_to_string(const NZVector<T>&, std::ostringstream&);
template <class T>
std::string vec_to_string(const NZVector<T>&);

// Inserisce alla fine di un vettore i coefficienti contenuti in forma testuale
// in uno string stream.
// Quando usato per inizializzare un NZVector, il compilatore elide la copia
// (NRVO) costruendo direttamente il vettore di destinazione.
template <class T>
void string_to_vec(std::istringstream&, NZVector<T>&);

template <class T>
void string_to_vec(const std::string&, NZVector<T>&);

}  // namespace tool

#include "../src/tool.inl"
#endif  // TOOL_H