#include <algorithm>  // merge algorithm and copy
#include <chrono>
#include <complex>
#include <filesystem>  //needs -std=c++17 to be compiled
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>  //ostream_iterator()
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>  // vector class-template definition
#include "../inc/NZVector.hpp"
#include "../inc/tool.hpp"

using namespace std;
namespace fs = std::filesystem;

enum exe_request { FILE_INPUT = 1, RANDOM_INPUT, MANUAL, TEST, END };
unsigned short GetRequest();

int main()
{
  unsigned short user_choice = GetRequest();
  while (user_choice != END) {
    switch (user_choice) {
      case FILE_INPUT: {
        break;
      }
      case RANDOM_INPUT: {
        break;
      }
      case MANUAL: {
        break;
      }
      case TEST: {
        NZVector<complex<double>> a;
        NZVector<complex<double>> b(a);
        NZVector<complex<double>> c(move(a));
        NZVector<double> r;
        string s;
        tool::get_input(s);
        cout << s << '\n';
        tool::rand_to_vec(b, 50, 0);
        for (int i = 0; i < 10; ++i) b.push_back(0.);
        b.push_back({9., 5.});
        b.set(0, 0.);
        cout << "\nb[0] = " << b.at(0) << "\nb[60] = " << b.at(60) << "\n"
             << '\n';
        b.print();

        b.set(0, {4., 6.});
        cout << "\nb[0] = " << b.at(0) << "\nb[60] = " << b.at(60) << '\n';
        b.print();

        b.set(0, [](complex<double>& val) { val = {56., 87.4}; });
        cout << "\nb[0] = " << b.at(0) << "\nb[60] = " << b.at(60) << '\n';
        b.print();
        // NZVector<double> a;
        // NZVector<double> b(a);
        // NZVector<double> c(move(a));

        /*         b = c;
                c = move(b);
                c.push_back({2, 3});
                c.push_back({0, 1});
                c.print();
                a.print();
                b.print();
         */
        break;
      }
    }
    user_choice = GetRequest();
  }

  return 0;
}

// permette all'utente di realizzare la scelta del metodo di rimpimento della
// matrice
unsigned short GetRequest()
{
  short user_choice;
  // Mostra minima veste grafica al menu
  cout << "\n\033[1;41;43;7m  Silver Solver\033[41;43;7m               "
          "      \033[0m\n"
       //  << "\033[41;43m                                    \033[0m\n"
       << "\033[41;43m  \033[41;43mEnter your choice                 "
          "\033[0m\n"
       << "\033[41;43m  [1]\033[1;41;43m Retrieve data from files      "
          "\033[0m\n"
       << "\033[41;43m  [2]\033[1;41;43m Generate random coefficients  "
          "\033[0m\n"
       << "\033[41;43m  [3]\033[1;41;43m Manual                        "
          "\033[0m\n"
       << "\033[41;43m  [4]\033[1;41;43m Test                          "
          "\033[0m\n"
       << "\033[41;43m  [5]\033[1;41;43m Quit                          "
          "\033[0m\n"
       << "\033[41;43m                                    \033[0m\n";

  do
    tool::get_input(user_choice, std::cin);
  while (user_choice < FILE_INPUT || user_choice > END);

  return user_choice;
}
