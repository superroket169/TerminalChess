# TerminalChess
A two-player chess game that runs entirely in the terminal (CLI).

> [!Warning]
> have no any interface based on graphics or CLI

## Status
- added moveValidator, makeMove, getGameState functions
- already can playable with your friends

## Features
- Two-player local chess
- CheckMate, Stealmate controls
- have these : enpassant, castlings, promotions

## Tech Stack
- C++

## Planned Features
- Board rendering enhancements
- Adding a chessBot uses bruteForce & tree

## how to run

```bash
g++ ./src/Chess/Game.cpp ./src/Chess/Chess.cpp -o gameCLI
```

