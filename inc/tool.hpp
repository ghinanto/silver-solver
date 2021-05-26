#ifndef TOOL_H
#define TOOL_H

#include <limits>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

namespace tool {

template <class T>
bool is_zero(const T&);

void rand_to_vec(NZVector<double>&, long);

// complex_on_tot percentuale tra 0 e 100
void rand_to_vec(NZVector<complex<double>>&, long, short complex_on_tot);

}  // namespace tool

#include "../src/tool.inl"
#endif  // TOOL_H