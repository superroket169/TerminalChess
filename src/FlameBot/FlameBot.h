#pragma once
#include "../Chess/Chess.h"
#include <vector>

namespace FlameBoth
{
    const int INF = 1000000;
    const int MATE_SCORE = 30000;

    enum class EvulateValue 
    { 
        PawnValue = 100, 
        KnightValue = 320, 
        BishopValue = 330, 
        RookValue = 500, 
        QueenValue = 900, 
        MateValue = 20000 
    };

    class Bot
    {
    public:
        Bot() = default;
        Chess::Move getBestMove(Chess::Board board, int depth);

    private:
        int searchTree(Chess::Board board, int depth, int alpha, int beta, bool isMaximizing);
        int evaluate(const Chess::Board& board);
        std::vector<Chess::Move> getAllValidMoves(const Chess::Board& board, Chess::Side side);
    };
}