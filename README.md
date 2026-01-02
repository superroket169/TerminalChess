# FlameBot Chess Engine

FlameBot is a terminal-based chess engine written in C++. 

## Features

* **Search Algorithm:** Minimax with Alpha-Beta Pruning.
* **Optimization:** Move Ordering for faster cut-offs.
* **Memory:** Transposition Table using String Hashing to cache board states.
* **Time Management:** Iterative Deepening with a strict time limit (default: 5s).
* **Interface:** Clean Command Line Interface (CLI).

## Build Instructions

* You need a C++ compiler that supports **C++17** (GCC, MinGW, or Clang).
* You can build it on your linux or windows.

### How to build
Run the following command in the project root:
* for linux : 
```bash
cmake -S . -B build
```
* for windows :
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -S . -B build
```

## How code works
* **FlameBoth::Bot::evaluate(..)** this funct calculates boards state
* **MATE_SCORE** this macro just works if there is a mate. thats value is so high
* **Alpha-Beta Pruning** optimizates all code. dont look needless tree branches
* **Move Ordering** this system orders moves for optimizate Alpha-Beta pruning look for ```src/FlameBot/FlameBot.cpp:120```
* **String Hashing** tanks to hashing FlameBot's speed increases two times. note : should be optimizate hashing