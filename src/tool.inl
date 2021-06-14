#include <cmath>    // abs(double)
#include <complex>  // abs(complex)
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <tuple>
#include <vector>
#include "../inc/tool.hpp"

template <class T>
void tool::get_input(T& variable, std::istream& input, std::ostream& output)
{
  output << "\n? ";
  input >> variable;

  while (!input) {
    if (input.eof()) {
      output << "Input stream closed\n";
      exit(EXIT_FAILURE);
    }
    output << "\n? ";
    // Everything such as ".", "," puts cin in error state, thus
    // require  clear.
    input.clear();
    //"8.9" or  "94ybib" is not a cin error. Initialize w\ 8
    // but leavs ".9" in stream, thus
    // require  ignore.
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    input >> variable;
  }
  input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <class T>
bool tool::is_zero(const T& value)
{
  if (std::abs(value) < std::numeric_limits<T>::epsilon()) return true;
  return false;
}

template <>
bool tool::is_zero<std::complex<double>>(const std::complex<double>& value)
{
  if (std::abs(value.real()) < std::numeric_limits<double>::epsilon() &&
      std::abs(value.imag()) < std::numeric_limits<double>::epsilon())
    return true;
  return false;
}

void tool::rand_to_vec(NZVector<double>& vec, long size)
{
  if (size < 0)
    throw std::invalid_argument(
        "rand_to_vec: negative amount of rows or columns");
  vec.clear();
  vec.reserve(size);

  // Boundaries User Input
  double coeff_min{0.}, coeff_max{0.};

  std::cout
      << "\nCoefficients are gonna be generated uniformly between two real "
         "numbers"
      << "\nEnter first boundary";
  tool::get_input(coeff_min, std::cin);

  std::cout << "\nEnter second boundary";
  tool::get_input(coeff_max, std::cin);

  // Generation
  std::mt19937 gen;  // generatore dei coeff
  std::uniform_real_distribution<double> dis_coeff(coeff_min, coeff_max);
  std::uniform_int_distribution<short> dis_coin(0, 100);

  // eseguo seed, fornisco alternativa sicura se primo metodo(migliore) non
  // funzia
  try {                     // random_device may be unavailable
    std::random_device rd;  // generatore dei seed
    gen.seed(rd());
  } catch (std::exception& e) {
    typedef std::chrono::high_resolution_clock clockHR;
    clockHR::time_point beginning = clockHR::now();
    std::cerr << "\nexception caught: " << e.what() << '\n';
    clockHR::duration d = clockHR::now() - beginning;
    unsigned seed_timer = d.count();  // get seed based on timer

    gen.seed(seed_timer);
  }

  for (long i{0}; i < size; ++i) {
    double val{dis_coeff(gen)};
    vec.push_back(val);
  }
}

void tool::rand_to_vec(NZVector<std::complex<double>>& vec,
                       long size,
                       short complex_on_tot)
{
  if (size < 0)
    throw std::invalid_argument(
        "rand_to_vec: negative amount of rows or columns");
  vec.clear();
  vec.reserve(size);

  // Boundaries User Input
  double coeff_min{0.}, coeff_max{0.};

  std::cout
      << "\nCoefficients are gonna be generated uniformly between two real "
         "numbers"
      << "\nEnter first boundary";
  tool::get_input(coeff_min, std::cin);

  std::cout << "\nEnter second boundary";
  tool::get_input(coeff_max, std::cin);

  // Generation
  std::mt19937 gen;  // generatore dei coeff
  std::uniform_real_distribution<double> dis_coeff(coeff_min, coeff_max);
  std::uniform_int_distribution<short> dis_coin(0, 100);

  // eseguo seed, fornisco alternativa sicura se primo metodo(migliore) non
  // funzia
  try {                     // random_device may be unavailable
    std::random_device rd;  // generatore dei seed
    gen.seed(rd());
  } catch (std::exception& e) {
    typedef std::chrono::high_resolution_clock clockHR;
    clockHR::time_point beginning = clockHR::now();
    std::cerr << "\nexception caught: " << e.what() << '\n';
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
