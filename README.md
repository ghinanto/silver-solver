# Silver Solver
Risolve sistemi lineari a coefficienti reali o complessi e mostra le possibili soluzioni nel caso di sistemi indeterminati.
## Dependencies
Standard Library del C++.
In particolare è necessario g++-10 per utilizzare la concepts library.
Per installarlo su Ubuntu:
```
$ sudo apt install gcc-10
```
Viene utilizzato Cmake per produrre i file di ninja, e ninja per compilare.
Per installarli su Ubuntu:
```
$ sudo apt install cmake ninja-build
```
## Build
Dalla top directory
```
$ ls
CMakeLists.txt inc src
$ mkdir build
$ cd build
$ cmake -G Ninja -D CMAKE_CXX_COMPILER=g++-10 ../
$ ninja
```
Quindi per invocare il programma
```
$ ./silver-solver
```