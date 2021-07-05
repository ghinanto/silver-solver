// Copyright 2021, Antonio Ghinassi, antonio.ghinassi@studio.unibo.it
#include <algorithm>
#include <complex>
#include <concepts>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../inc/Matrix.hpp"
#include "../inc/NZVector.hpp"
#include "../inc/tool.hpp"
namespace fs = std::filesystem;

enum exe_request { FILE_INPUT = 1, RANDOM_INPUT, END };
unsigned short GetRequest();

int main()
{
  unsigned short user_choice = GetRequest();
  while (user_choice != END) {
    std::string terms_file;
    std::string matrix_file;
    bool complex_field{false};

    // Mostra su output la soluzione del sistema
    auto sol_out = [&complex_field](std::ranges::range auto& sol_set,
                                    std::ranges::range auto& sol_idx,
                                    std::ostream& out) {
      if (sol_set.size() == 0)
        throw std::invalid_argument("Main::sol_out: Il vettore è vuoto");
      std::size_t n_pars{sol_set.at(0).size() - 1};
      std::vector<long> pars_idx;
      pars_idx.reserve(n_pars);
      std::ostringstream s_line;

      // Verifica se variabili interne sono parametri, ovvero il loro indice non
      // appare in sol_idx
      // es. sol_idx = {9, 8, 7, 4, 3, 2, 0}
      //     x[6], x[5], x[1] sono parametri
      for (long p_idx{0}, max_idx{sol_idx.at(0)}; p_idx < max_idx; ++p_idx)
        if (std::none_of(sol_idx.begin(), sol_idx.end(), [p_idx](long idx) {
              return idx == p_idx;
            })) {
          pars_idx.push_back(p_idx);
          --n_pars;
        }
      // Inoltre sono parametri anche le variabili con indice maggiore del
      // massimo contenuto in sol_idx
      for (long p_idx{0}, end{static_cast<long>(sol_idx.size())};
           p_idx < n_pars;
           ++p_idx)
        pars_idx.push_back(end + p_idx);
      // Le righe mostrano le componenti del vettore soluzione
      long i{0};
      for (const auto& sol : sol_set) {
        s_line.str(std::string());  // Svuota lo stringstream
        s_line << std::noshowpos;
        s_line << "\nx[" << sol_idx.at(i++) << "] = ";

        auto pars_idx_it = pars_idx.rbegin();
        for (auto coeff_it = sol.begin(); coeff_it != sol.end(); ++coeff_it) {
          s_line << std::scientific << std::left << std::setprecision(4)
                 << std::showpos;
          s_line << *coeff_it;

          if (coeff_it != sol.begin())
            s_line << std::internal << std::noshowpos << "*x["
                   << *(pars_idx_it++) << "]";
          s_line << "  ";
        }
        out << s_line.str();
      }
    };

    // Gestisce la scrittura su file del vettore soluzione
    auto sol_to_file = [&sol_out](std::ranges::range auto& sol_set,
                                  std::ranges::range auto& sol_idx) {
      std::cout << "\n\nInserire il nome del file su cui salvare la soluzione "
                   "altrimenti premere invio."
                << "\nSe non incluso un percorso, il file viene salvato in "
                << fs::current_path();
      std::string sol_file;
      std::cout << "\n? ";
      std::getline(std::cin, sol_file);
      if (sol_file.size()) {
        std::ofstream sol_fstream(sol_file);
        if (!sol_fstream)
          throw std::ios_base::failure("Non è stato possibile aprire il file " +
                                       sol_file);

        sol_out(sol_set, sol_idx, sol_fstream);
      }
    };

    switch (user_choice) {
      case FILE_INPUT: {
        std::cout << "\n\033[41;43;7m"
                     "[1]\033[0;41;43;1m"
                     " Leggi coefficienti da file di testo "
                     "\033[0m";
        std::cout << "\nIl sistema da risolvere ha coefficienti"
                  << "\n [0] reali"
                  << "\n [1] complessi";
        tool::get_input(complex_field);

        std::cout << "\nInserire il nome del file che contiene i TERMINI NOTI, "
                     "poi premere invio."
                  << "\nSe non incluso un percorso, il file viene cercato in "
                  << fs::current_path();
        std::cout << "\n? ";
        // Permette di gestire nomi di file che contengono spazi
        std::getline(std::cin, terms_file);

        std::cout
            << "\nInserire il nome del file che contiene i coefficienti della "
               "MATRICE, poi premere invio"
            << "\nSe non incluso un percorso, il file viene cercato in "
            << fs::current_path();
        std::cout << "\n? ";
        // Permette di gestire nomi di file che contengono spazi
        std::getline(std::cin, matrix_file);
        std::cout << "Elaborazione in corso. Attendere ..." << std::endl;

        try {
          if (complex_field) {
            NZVector<std::complex<double>> terms(terms_file);
            Matrix<std::complex<double>> mat(matrix_file);
            auto sol = mat.solve(terms);
            auto& sol_set = std::get<0>(sol);
            auto& sol_idx = std::get<1>(sol);

            if (not sol_set.size())
              std::cout << "\nIl sistema non ha soluzione.";
            else {
              sol_out(sol_set, sol_idx, std::cout);
              sol_to_file(sol_set, sol_idx);
            }

          } else {
            NZVector<double> terms(terms_file);
            Matrix<double> mat(matrix_file);
            auto sol = mat.solve(terms);

            auto& sol_set = std::get<0>(sol);
            auto& sol_idx = std::get<1>(sol);

            if (not sol_set.size())
              std::cout << "\nIl sistema non ha soluzione.";
            else {
              sol_out(sol_set, sol_idx, std::cout);
              sol_to_file(sol_set, sol_idx);
            }
          }
        } catch (std::exception& e) {
          std::cout << "\nErrore: " << e.what();
        }

        break;
      }
      case RANDOM_INPUT: {
        std::cout << "\n\033[41;43;7m"
                     "[2]\033[0;41;43;1m"
                     " Genera coefficienti casuali "
                     "\033[0m";
        long rows{0}, cols{0};
        short complex_on_tot{0};
        double first_bound{0}, second_bound{0};

        std::cout << "\nInserire il numero di equazioni";
        do
          tool::get_input(rows);
        while (rows < 0);
        std::cout << "\nInserire il numero di incognite";
        do
          tool::get_input(cols);
        while (cols < 0);

        std::cout << "\nInserire la percentuale di coefficienti con parte "
                     "immaginaria diversa da zero da generare"
                  << "\nInserire 0 per generare coefficienti reali";
        do
          tool::get_input(complex_on_tot);
        while (complex_on_tot < 0 || complex_on_tot > 100);
        if (complex_on_tot) complex_field = true;

        complex_field
            ? std::cout << "\nLe parti reali e immaginarie dei coefficienti "
                           "vengono estratte da un intervallo reale"
            : std::cout
                  << "\nI coefficienti vengono estratti da un intervallo reale";

        std::cout << "\nInserisci il primo estremo dell'intervallo";
        tool::get_input(first_bound);
        std::cout << "\nInserisci il secondo estremo dell'intervallo";
        tool::get_input(second_bound);

        std::cout
            << "\nPer salvare la MATRICE generata inserire il nome del file "
               ",altrimenti premere invio."
            << "\nSe non incluso un percorso, il file viene salvato in "
            << fs::current_path();
        std::cout << "\n? ";
        // Permette di gestire nomi di file che contengono spazi
        std::getline(std::cin, matrix_file);

        std::cout << "\nPer salvare i TERMINI NOTI generati inserire il nome "
                     "del file, altrimenti premere invio."
                  << "\nSe non incluso un percorso, il file viene salvato in "
                  << fs::current_path();
        std::cout << "\n? ";
        // Permette di gestire nomi di file che contengono spazi
        std::getline(std::cin, terms_file);
        std::cout << "Elaborazione in corso. Attendere ..." << std::endl;

        try {
          if (complex_field) {
            NZVector<std::complex<double>> terms = tool::rand_to_vec(
                rows, complex_on_tot, first_bound, second_bound);
            Matrix<std::complex<double>> mat(
                rows, cols, complex_on_tot, first_bound, second_bound);

            auto sol = mat.solve(terms);
            auto& sol_set = std::get<0>(sol);
            auto& sol_idx = std::get<1>(sol);

            if (not sol_set.size())
              std::cout << "\nIl sistema non ha soluzione.";
            else {
              sol_out(sol_set, sol_idx, std::cout);
              sol_to_file(sol_set, sol_idx);
            }

            if (matrix_file.size()) mat.to_file(matrix_file);
            if (terms_file.size()) tool::vec_to_file(terms, terms_file);

          } else {
            NZVector<double> terms =
                tool::rand_to_vec(rows, first_bound, second_bound);
            Matrix<double> mat(rows, cols, first_bound, second_bound);
            auto sol = mat.solve(terms);
            auto& sol_set = std::get<0>(sol);
            auto& sol_idx = std::get<1>(sol);

            if (not sol_set.size())
              std::cout << "\nIl sistema non ha soluzione.";
            else {
              sol_out(sol_set, sol_idx, std::cout);
              sol_to_file(sol_set, sol_idx);
            }

            if (matrix_file.size()) mat.to_file(matrix_file);
            if (terms_file.size()) tool::vec_to_file(terms, terms_file);
          }
        } catch (std::exception& e) {
          std::cout << "\nErrore: " << e.what();
        }
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
  std::cout << "\n\033[1;41;43;7m  Silver Solver\033[41;43;7m                "
               "                "
               "\033[0m\n"
            << "\033[41;43m  \033[41;43mRisolutore di sistemi di equazioni "
               "lineari.  "
               "\033[0m\n"
            << "\033[41;43m                                               "
               "\033[0m\n"
            << "\033[41;43m  \033[41;43mScegliere un opzione                 "
               "        "
               "\033[0m\n"
            << "\033[41;43m  [1]\033[1;41;43m Leggi coefficienti da file di "
               "testo      "
               "\033[0m\n"
            << "\033[41;43m  [2]\033[1;41;43m Genera coefficienti casuali    "
               "          "
               "\033[0m\n"
            << "\033[41;43m  [3]\033[1;41;43m Esci                           "
               "          "
               "\033[0m\n"
            << "\033[41;43m                                               "
               "\033[0m\n";

  do
    tool::get_input(user_choice, std::cin);
  while (user_choice < FILE_INPUT || user_choice > END);

  return user_choice;
}
