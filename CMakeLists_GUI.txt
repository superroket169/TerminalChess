cmake_minimum_required(VERSION 3.21)
project(TerminalChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SLINT_DIR "${CMAKE_SOURCE_DIR}/../Slint-cpp-1.14.1-Linux-x86_64")
set(CMAKE_PREFIX_PATH ${SLINT_DIR})

find_package(Slint REQUIRED)

add_executable(TerminalChess
  src/gui.cpp
  src/ChessApp/ChessApp.cpp
  src/Time/Time.cpp
  src/Chess/Chess.cpp
  src/FlameBot/FlameBot.cpp
  src/BoardHash/BoardHash.cpp
)

slint_target_sources(TerminalChess ui/appGui.slint)

target_include_directories(TerminalChess PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(TerminalChess PRIVATE Slint::Slint)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS ">>> Target System: Windows (Static Build)")
    target_compile_options(TerminalChess PRIVATE -O3 -funroll-loops -Wall)
    target_link_options(TerminalChess PRIVATE -static -static-libgcc -static-libstdc++)

else()
    message(STATUS ">>> Target System: Linux/Unix (Native Optimization)")
    target_compile_options(TerminalChess PRIVATE -O3 -march=native -funroll-loops -Wall)
endif()