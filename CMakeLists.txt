cmake_minimum_required (VERSION 3.8)

project(silver-solver VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(silver-solver src/main.cpp)
add_compile_options(-Wall -Wextra -fsanitize=address)