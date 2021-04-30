#include <cmath>    // abs(double)
#include <complex>  // abs(complex)
#include <limits>
#include <string>
#include <tuple>
#include <vector>
#include "../inc/tool.hpp"

template <class T>
bool tool::is_zero(const T& value)
{
  if (abs(value) < numeric_limits<T>::epsilon()) return true;
  return false;
}

template <>
bool tool::is_zero<complex<double>>(const complex<double>& value)
{
  if (abs(value.real()) < numeric_limits<double>::epsilon() &&
      abs(value.imag()) < numeric_limits<double>::epsilon())
    return true;
  return false;
}