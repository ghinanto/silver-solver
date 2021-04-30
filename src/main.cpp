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

        // NZVector<double> a;
        // NZVector<double> b(a);
        // NZVector<double> c(move(a));

        b = c;
        c = move(b);
        c.push_back({2, 3});
        c.push_back({0, 1});
        c.print();
        a.print();
        b.print();
        cout << '\n' << c.at(0) << '\n';
        break;
      }
    }
    user_choice = GetRequest();
  }
}

// permette all'utente di realizzare la scelta del metodo di rimpimento della
// matrice
unsigned short GetRequest()
{
  long user_choice;
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

  do {  // input user request
    cout << "\n? ";
    cin >> user_choice;

    if (cin.eof()) {
      cerr << "Input stream closed\n";
      exit(EXIT_FAILURE);  //?
    } else if (!cin) {
      cin.clear();
      user_choice = 0;  // null state: do-while loop goes on
    }
    cin.ignore(numeric_limits<streamsize>::max(),
               '\n');  //"8.9" is not a cin error, thus require ignore.
                       // Everything else such as ".", "," or too long ints put
                       // cin in error state, thus require clear.

  } while (user_choice < FILE_INPUT || user_choice > END);

  return user_choice;
}
