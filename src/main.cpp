#include <algorithm>  // merge algorithm and copy
#include <chrono>
#include <complex>
#include <filesystem>  //needs -std=c++17 to be compiled
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>  //ostream_iterator()
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>  // vector class-template definition
#include "../inc/Matrix.hpp"
#include "../inc/NZVector.hpp"
#include "../inc/tool.hpp"

namespace fs = std::filesystem;

enum exe_request { FILE_INPUT = 1, RANDOM_INPUT, MANUAL, TEST, END };
unsigned short GetRequest();

int main()
{
  unsigned short user_choice = GetRequest();
  while (user_choice != END) {
    std::string terms_file;
    std::string matrix_file;
    std::string sol_file;
    bool complex_field{false};
    switch (user_choice) {
      case FILE_INPUT: {
        std::cout << "\nIl sistema da risolvere ha coefficienti"
                  << "\n [0] reali"
                  << "\n [1] complessi";
        tool::get_input(complex_field);

        std::cout << "\nInserire il nome del file che contiene i TERMINI NOTI, "
                     "poi premere invio."
                  << "\nPosizione attuale: " << fs::current_path()
                  << "\nSe i file si trovano in un'altra posizione, includere "
                     "il percorso assoluto o relativo.";
        std::cout << "\n? ";
        // Permette di gestire nomi di file che contengono spazi
        std::getline(std::cin, terms_file);

        std::cout
            << "\nInserire il nome del file che contiene i coefficienti della "
               "MATRICE, poi premere invio"
            << "\nPosizione attuale: " << fs::current_path()
            << "\nSe i file si trovano in un'altra posizione, includere "
               "il percorso assoluto o relativo";
        std::cout << "\n? ";
        // Permette di gestire nomi di file che contengono spazi
        std::getline(std::cin, matrix_file);

        if (complex_field) {
          NZVector<std::complex<double>> terms(terms_file);
          Matrix<std::complex<double>> mat(matrix_file);
          auto sol = mat.solve(terms);
          auto& sol_set = std::get<0>(sol);
          auto& sol_idx = std::get<1>(sol);

          int i{0};
          for (const auto& sol : sol_set) {
            std::cout << "\nx[" << sol_idx.at(i++) << "]:\n";
            for (const auto& coeff : sol) {
              std::cout << coeff << ", ";
            }
          }
        } else {
          NZVector<double> terms(terms_file);
          Matrix<double> mat(matrix_file);
          auto sol = mat.solve(terms);
          auto& sol_set = std::get<0>(sol);
          auto& sol_idx = std::get<1>(sol);

          int i{0};
          for (const auto& sol : sol_set) {
            std::cout << "\nx[" << sol_idx.at(i++) << "]:\n";
            for (const auto& coeff : sol) {
              std::cout << coeff << ", ";
            }
          }

          if (not sol_set.size()) std::cout << "\nIl sistema non ha soluzione.";
        }

        break;
      }
      case RANDOM_INPUT: {
        break;
      }
      case MANUAL: {
        break;
      }
      case TEST: {
        Matrix<std::complex<double>> mat("../txt/firstfileever");
        mat.to_file("../txt/m_rand.txt");

        NZVector<std::complex<double>> c_terms({{2.}, {0., 3.}});

        std::vector<std::vector<std::complex<double>>> sol_set;
        std::vector<long> sol_idx;
        // mat.print();
        // c_terms.print();
        std::tie(sol_set, sol_idx) = mat.solve(c_terms);
        std::clog << "\nSOL_IDX SIZE = " << sol_idx.size()
                  << "\nSOL_SET SIZE = " << sol_set.size();
        int i{0};
        for (const auto& sol : sol_set) {
          std::cout << "\nx[" << sol_idx.at(i++) << "]:\n";
          for (const auto& coeff : sol) {
            std::cout << coeff << ", ";
          }
        }
        std::clog << "\nSOL_IDX:\n";
        for (const long& idx : sol_idx) std::clog << idx;
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
  std::cout
      << "\n\033[1;41;43;7m  Silver Solver\033[41;43;7m                        "
         "        "
         "\033[0m\n"
      //  << "\033[41;43m                                    \033[0m\n"
      << "\033[41;43m  \033[41;43mScegliere un opzione                         "
         "\033[0m\n"
      << "\033[41;43m  [1]\033[1;41;43m Leggi coefficienti da file di testo    "
         "  "
         "\033[0m\n"
      << "\033[41;43m  [2]\033[1;41;43m Genera coefficienti casuali            "
         "  "
         "\033[0m\n"
      << "\033[41;43m  [3]\033[1;41;43m Manual                                 "
         "  "
         "\033[0m\n"
      << "\033[41;43m  [4]\033[1;41;43m Test                                   "
         "  "
         "\033[0m\n"
      << "\033[41;43m  [5]\033[1;41;43m Esci                                   "
         "  "
         "\033[0m\n"
      << "\033[41;43m                                               "
         "\033[0m\n";

  do
    tool::get_input(user_choice, std::cin);
  while (user_choice < FILE_INPUT || user_choice > END);

  return user_choice;
}
