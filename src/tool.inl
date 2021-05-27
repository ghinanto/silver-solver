#include <cmath>    // abs(double)
#include <complex>  // abs(complex)
#include <iterator>
#include <limits>
#include <string>
#include <tuple>
#include <vector>
#include "../inc/tool.hpp"

// base() restituisce un forward iterator che punta l'elemento che segue quello
// che il reverse iterator stava puntando
/* template <class ReverseIterator>
typename ReverseIterator::iterator_type make_forward(ReverseIterator rit)
{
  if (rit == rend())
    ;
  return --(rit.base());  // move result of .base() back by one.
                          // alternatively
                          // return (++rit).base() ;
                          // or
                          // return (rit+1).base().
} */

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

void tool::rand_to_vec(NZVector<double>& vec, long size)
{
  if (size < 0)
    throw invalid_argument("rand_to_vec: negative amount of rows or columns");
  vec.clear();
  vec.reserve(size);

  // Boundaries User Input
  double coeff_min{0.}, coeff_max{0.};

  cout << "\nCoefficients are gonna be generated uniformly between two real "
          "numbers"
       << "\nEnter first boundary"
       << "\n? ";
  cin >> coeff_min;

  while (!cin) {
    if (cin.eof()) {
      cerr << "Input stream closed\n";
      exit(EXIT_FAILURE);
    }
    cout << "\n? ";
    // Everything such as ".", "," puts cin in error state, thus
    // require  clear.
    cin.clear();
    //"8.9" or  "94ybib" is not a cin error. Initialize w\ 8
    // but leavs ".9" in stream, thus
    // require  ignore.
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cin >> coeff_min;
  }

  cout << "\nEnter second boundary"
       << "\n? ";
  cin >> coeff_max;

  while (!cin) {
    if (cin.eof()) {
      cerr << "Input stream closed\n";
      exit(EXIT_FAILURE);  //?
    }
    cout << "\n? ";
    // Everything such as ".", "," puts cin in error state, thus
    // require  clear.
    cin.clear();
    //"8.9" or  "94ybib" is not a cin error. Initialize w\ 8
    // but leavs ".9" in stream, thus
    // require  ignore.
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cin >> coeff_max;
  }

  // Generation
  mt19937 gen;  // generatore dei coeff
  uniform_real_distribution<double> dis_coeff(coeff_min, coeff_max);
  uniform_int_distribution<short> dis_coin(0, 100);

  // eseguo seed, fornisco alternativa sicura se primo metodo(migliore) non
  // funzia
  try {                // random_device may be unavailable
    random_device rd;  // generatore dei seed
    gen.seed(rd());
  } catch (exception& e) {
    typedef std::chrono::high_resolution_clock clockHR;
    clockHR::time_point beginning = clockHR::now();
    cerr << "\nexception caught: " << e.what() << '\n';
    clockHR::duration d = clockHR::now() - beginning;
    unsigned seed_timer = d.count();  // get seed based on timer

    gen.seed(seed_timer);
  }

  for (long i{0}; i < size; ++i) {
    double real{dis_coeff(gen)};
    vec.push_back(real);
  }
}

void tool::rand_to_vec(NZVector<complex<double>>& vec,
                       long size,
                       short complex_on_tot)
{
  if (size < 0)
    throw invalid_argument("rand_to_vec: negative amount of rows or columns");
  vec.clear();
  vec.reserve(size);

  // Boundaries User Input
  double coeff_min = 0., coeff_max = 0.;

  cout << "\nCoefficients are gonna be generated uniformly between two real "
          "numbers"
       << "\nEnter first boundary"
       << "\n? ";
  cin >> coeff_min;

  while (!cin) {
    if (cin.eof()) {
      cerr << "Input stream closed\n";
      exit(EXIT_FAILURE);  //?
    }
    cout << "\n? ";
    cin.clear();  // Everything such as ".", "," put cin in error state, thus
                  // require  clear.
    cin.ignore(std::numeric_limits<std::streamsize>::max(),
               '\n');  //"8.9" or  "94ybib" is not a cin error. Initialize w\ 8
                       // but leavs ".9" in stream, thus
                       // require  ignore.

    cin >> coeff_min;
  }

  cout << "\nEnter second boundary"
       << "\n? ";
  cin >> coeff_max;

  while (!cin) {
    if (cin.eof()) {
      cerr << "Input stream closed\n";
      exit(EXIT_FAILURE);  //?
    }
    cout << "\n? ";
    cin.clear();  // Everything such as ".", "," put cin in error state, thus
                  // require  clear.
    cin.ignore(std::numeric_limits<std::streamsize>::max(),
               '\n');  //"8.9" or  "94ybib" is not a cin error. Initialize w\ 8
                       // but leavs ".9" in stream, thus
                       // require  ignore.

    cin >> coeff_max;
  }

  // Generation
  mt19937 gen;  // generatore dei coeff
  uniform_real_distribution<double> dis_coeff(coeff_min, coeff_max);
  uniform_int_distribution<short> dis_coin(0, 100);

  // eseguo seed, fornisco alternativa sicura se primo metodo(migliore) non
  // funzia
  try {                // random_device may be unavailable
    random_device rd;  // generatore dei seed
    gen.seed(rd());
  } catch (exception& e) {
    typedef std::chrono::high_resolution_clock clockHR;
    clockHR::time_point beginning = clockHR::now();
    cerr << "\nexception caught: " << e.what() << '\n';
    clockHR::duration d = clockHR::now() - beginning;
    unsigned seed_timer = d.count();  // get seed based on timer

    gen.seed(seed_timer);
  }

  for (long i{0}; i < size; ++i) {  // complex
    double real{dis_coeff(gen)};
    double imag{0.};
    if (dis_coin(gen) < complex_on_tot) imag = dis_coeff(gen);
    vec.push_back({real, imag});
  }
}
