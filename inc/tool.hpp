#ifndef TOOL_H
#define TOOL_H

#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

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
template <class T>
bool is_zero(const T&);

// Riempie un vettore di dimensione 'size' con valori random
void rand_to_vec(NZVector<double>&, long size);
// complex_on_tot percentuale tra 0 e 100
void rand_to_vec(NZVector<complex<double>>&, long, short complex_on_tot);

}  // namespace tool

#include "../src/tool.inl"
#endif  // TOOL_H