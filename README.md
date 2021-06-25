# Silver Solver
Risolve sistemi lineari qualsiasi a coefficienti reali o complessi.
## Dependencies
Standard Library del C++.
In particolare è ecessario g++-10 per utilizzare la concepts library.
Per installarlo su Ubuntu:
sudo apt install gcc-10
## Build
Viene utilizzato Cmake per produrre i file di ninja, e ninja per compilare. 
Dalla top directory
```
mkdir build
cd build
cmake -G Ninja -D CMAKE_CXX_COMPILER=g++-10 ../
ninja
```
