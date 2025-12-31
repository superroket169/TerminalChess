#include <iostream>
#include <string>
#include <vector>
#include "Chess/Chess.h"
#include "FlameBot/FlameBot.h"
#include "Time/Time.h"

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

char getPieceChar(Chess::Piece type, Chess::Side side)
{
    if (type == Chess::Piece::Empty) return '.';
    char c = '?';
    switch (type)
    {
        case Chess::Piece::Pawn:   c = 'p'; break;
        case Chess::Piece::Rook:   c = 'r'; break;
        case Chess::Piece::Knight: c = 'n'; break;
        case Chess::Piece::Bishop: c = 'b'; break;
        case Chess::Piece::Queen:  c = 'q'; break;
        case Chess::Piece::King:   c = 'k'; break;
        default: break;
    }
    return (side == Chess::Side::White) ? toupper(c) : c;
}

void printBoard(const Chess::Board& board)
{
    std::cout << "\n  +-----------------+\n";
    for (int r = 8; r >= 1; --r)
    {
        std::cout << r << " | ";
        for (int f = 1; f <= 8; ++f)
        {
            Chess::Square sq = board.getSquare({ (Chess::File)f, (Chess::Rank)r });
            std::cout << getPieceChar(sq.getPieceType(), sq.getPieceSide()) << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "  +-----------------+\n    A B C D E F G H\n\n";
}

bool parseCoordinate(std::string s, Chess::BoardCoordinate& outCoord)
{
    if (s.length() < 2) return false;
    char fileChar = tolower(s[0]);
    char rankChar = s[1];
    if (fileChar < 'a' || fileChar > 'h') return false;
    if (rankChar < '1' || rankChar > '8') return false;
    outCoord.file = (Chess::File)(fileChar - 'a' + 1);
    outCoord.rank = (Chess::Rank)(rankChar - '0');
    return true;
}

int main()
{
    std::cout << "=== FlameBoth Engine Baslatiliyor ===\n";

    Chess::Board board;
    FlameBoth::Bot bot;
    
    // Bot Ayarları
    int botDepth = 7;
    Chess::Side playerSide = Chess::Side::White;
    Chess::Side botSide = Chess::Side::Black;

    std::cout << "Sen: Beyaz | Bot: Siyah\n";

    while (true)
    {
        printBoard(board);

        Chess::GameState state = Chess::getGameState(board);
        if (state == Chess::GameState::Checkmate)
        {
            std::cout << "!!! SAH MAT !!! " << (board.getTurn() == Chess::Side::White ? "SIYAH" : "BEYAZ") << " KAZANDI!\n";
            break;
        }
        else if (state == Chess::GameState::Stalemate)
        {
            std::cout << "!!! PAT !!! OYUN BERABERE.\n";
            break;
        }

        if (board.getTurn() == playerSide)
        {
            std::string input;
            std::cout << "Hamleniz: ";
            std::cin >> input;

            if (input == "exit") break;
            if (input.length() != 4) { std::cout << "Gecersiz format!\n"; continue; }

            Chess::BoardCoordinate from, to;
            if (!parseCoordinate(input.substr(0, 2), from) || !parseCoordinate(input.substr(2, 2), to))
            {
                std::cout << "Gecersiz koordinat!\n"; continue;
            }

            Chess::Square sqFrom; sqFrom.setCoordinate(from);
            Chess::Square sqTo; sqTo.setCoordinate(to);
            Chess::Move move(sqFrom, sqTo);

            Chess::MoveType result = Chess::makeMove(move, board.getTurn(), board);
            if (result == Chess::MoveType::Invalid)      std::cout << ">>> Gecersiz Hamle!\n";
            else if (result == Chess::MoveType::inCheck) std::cout << ">>> Sah altindasin!\n";
            else board.passTurn();
        }
        else
        {
            std::cout << "FlameBoth dusunuyor... (Derinlik: " << botDepth << ")\n";
            
            Time timer;
            timer.start();
            Chess::Move bestMove = bot.getBestMove(board, botDepth);
            float timerTime = timer.elapsedTime();

            std::cout << "Hesaplama Suresi: " << timerTime << " saniye." << "\n";
            
            Chess::File f1 = bestMove.getFrom().getCoordinate().file;
            Chess::Rank r1 = bestMove.getFrom().getCoordinate().rank;
            Chess::File f2 = bestMove.getTo().getCoordinate().file;
            Chess::Rank r2 = bestMove.getTo().getCoordinate().rank;

            std::cout << "Bot Oynadi: " 
                      << (char)('a' + (int)f1 - 1) << (int)r1 
                      << (char)('a' + (int)f2 - 1) << (int)r2 << "\n";

            Chess::makeMove(bestMove, board.getTurn(), board);
            board.passTurn();
        }
    }
    return 0;
}