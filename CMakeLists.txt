cmake_minimum_required(VERSION 3.21)
project(TerminalChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(TerminalChess
  src/test.cpp
  src/Time/Time.cpp
  src/Chess/Chess.cpp
  src/FlameBot/FlameBot.cpp
  src/BoardHash/BoardHash.cpp
)

target_compile_options(TerminalChess PRIVATE -O3)

target_compile_options(TerminalChess PRIVATE
  $<$<CXX_COMPILER_ID:GNU>:-O3 -march=native -funroll-loops>
  $<$<CXX_COMPILER_ID:Clang>:-O3 -march=native -funroll-loops>
  $<$<CXX_COMPILER_ID:MSVC>:/O2>
)
