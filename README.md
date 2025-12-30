# TerminalChess
A two-player chess game that runs entirely in the terminal (CLI).

## Status
- added moveValidator, makeMove, getGameState functions
- already can playable with your friends
- already have a bot that can calculate 4 depth in 1-2 seconds

## Features
- Two-player local chess
- CheckMate, Stealmate controls
- have these : enpassant, castlings, promotions

### FlameBot
- The bot is just calculate most piece value
- I'll add central value for game begin
- it can calculate 4 depth in few seconds
- it can calculate 5 depth in 10-15 seconds

## Tech Stack
- C++

## Planned Features
- Board rendering enhancements
- Adding a chessBot uses bruteForce & tree

## how to build

```bash
g++ ./src/Chess/Game.cpp ./src/FlameBot/Game.cpp ./src/Chess/Chess.cpp -o gameCLI
```

