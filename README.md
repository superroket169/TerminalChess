# ♟️ FlameBot Chess Engine

FlameBot is a high-performance, modern chess engine written in **C++17**, now featuring a stunning **Neon-themed GUI** built with the **Slint UI framework**. It combines classical chess logic with a sleek cyberpunk aesthetic.

---

## ✨ Features

* **Modern GUI:** A reactive and sleek interface built with **Slint**.
* **Neon Visuals:** High-quality **SVG-based assets** for chess pieces, ensuring perfect rendering on all systems.
* **Search Algorithm:** Minimax with **Alpha-Beta Pruning** for deep position analysis.
* **Optimizations:**
    * **Move Ordering:** Prioritizes captures and checks to trigger faster Alpha-Beta cut-offs.
    * **Transposition Table:** Uses **Zobrist/String Hashing** to cache evaluations and speed up the search.
* **Time Management:** **Iterative Deepening** with strict time limits to ensure the bot plays optimally under pressure.

---

## 🛠️ Build Instructions

### Prerequisites
* **C++ Compiler:** GCC, Clang, or MinGW (must support **C++17**).
* **CMake:** Version 3.21 or higher.
* **Rust Toolchain:** (Handled automatically via Slint's `FetchContent`).

### Compilation

#### 🐧 For Linux:
```bash
# Set Rust flags for stability
export RUSTFLAGS="-A dangerous_implicit_autorefs"

# Configure and build
cmake -S . -B build
cmake --build build -j$(nproc)