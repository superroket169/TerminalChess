# FlameBot Chess Engine

FlameBot is a terminal-based chess engine written in C++. It utilizes tree search algorithms to provide a competitive experience against human players.

## Features

* **Search Algorithm:** Minimax with Alpha-Beta Pruning.
* **Optimization:** Move Ordering for faster cut-offs.
* **Memory:** Transposition Table using String Hashing to cache board states.
* **Time Management:** Iterative Deepening with a strict time limit (default: 5s).
* **Interface:** Clean Command Line Interface (CLI).

## Build Instructions

You need a C++ compiler that supports **C++17** (GCC, MinGW, or Clang).

### Linux (Ubuntu/Debian)
Run the following command in the project root:

```bash
g++ ./src/Game.cpp ./src/Time/Time.cpp ./src/Chess/Chess.cpp ./src/BoardHash/BoardHash.cpp ./src/FlameBot/FlameBot.cpp -std=c++17 -O3 -o gameCLI