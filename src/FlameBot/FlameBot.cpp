#include "FlameBot.h"
#include <algorithm>
#include <iostream>

Chess::Move FlameBoth::Bot::getBestMove(Chess::Board board, int depth)
{
    std::vector<Chess::Move> moves = getAllValidMoves(board, board.getTurn());
    
    Chess::Move bestMove;

    if (moves.empty()) return bestMove;

    bestMove = moves[0];

    bool isWhiteTurn = (board.getTurn() == Chess::Side::White);
    
    int bestVal = isWhiteTurn ? -INF : INF;
    
    for (const auto& move : moves)
    {
        Chess::Board tempBoard = board;
        Chess::makeMove(move, board.getTurn(), tempBoard);
        tempBoard.passTurn();
        
        int moveVal = searchTree(tempBoard, depth - 1, -INF, INF, !isWhiteTurn);
        
        if (isWhiteTurn)
        {
            // Beyaz: Daha büyük puan arar
            if (moveVal > bestVal)
            {
                bestVal = moveVal;
                bestMove = move;
            }
        }
        else
        {
            // Siyah: Daha küçük (negatif) puan arar
            if (moveVal < bestVal)
            {
                bestVal = moveVal;
                bestMove = move;
            }
        }
    }
    return bestMove;
}

int FlameBoth::Bot::searchTree(Chess::Board board, int depth, int alpha, int beta, bool isMaximizing)
{
    if(depth == 0) return evaluate(board);

    Chess::Side currentSide = board.getTurn();
    std::vector<Chess::Move> moves = getAllValidMoves(board, currentSide);

    if (moves.empty())
    {
        if (Chess::isKingInCheck(board, currentSide))
        {
            return isMaximizing ? -MATE_SCORE : MATE_SCORE;
        }
        else return 0; // Pat
    }

    std::sort(moves.begin(), moves.end(), [&](const Chess::Move& a, const Chess::Move& b){
        return fastMoveOrdering(a, board) > fastMoveOrdering(b, board);
    });

    if (isMaximizing) // White (default)
    {
        int maxEval = -INF;
        for (const auto& move : moves)
        {
            Chess::Board tempBoard = board;
            Chess::makeMove(move, currentSide, tempBoard);
            tempBoard.passTurn();

            int eval = searchTree(tempBoard, depth - 1, alpha, beta, false);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval); 

            if (beta <= alpha) break;
        }
        return maxEval;
    }
    else // Black (default)
    {
        int minEval = INF;
        for (const auto& move : moves)
        {
            Chess::Board tempBoard = board;
            Chess::makeMove(move, currentSide, tempBoard);
            tempBoard.passTurn();
            
            int eval = searchTree(tempBoard, depth - 1, alpha, beta, true);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval); 

            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int FlameBoth::Bot::evaluate(const Chess::Board& board)
{
    int whiteScore = 0;
    int blackScore = 0;

    for (int r = 1; r <= 8; ++r)
    {
        for (int f = 1; f <= 8; ++f)
        {
            Chess::Square sq = board.getSquare({(Chess::File)f, (Chess::Rank)r});
            if (sq.getPieceType() == Chess::Piece::Empty) continue;

            int pieceValue = 0;
            
            switch (sq.getPieceType())
            {
                case Chess::Piece::Pawn:   pieceValue = (int) EvulateValue::PawnValue ; break;
                case Chess::Piece::Knight: pieceValue = (int) EvulateValue::KnightValue ; break;
                case Chess::Piece::Bishop: pieceValue = (int) EvulateValue::BishopValue ; break;
                case Chess::Piece::Rook:   pieceValue = (int) EvulateValue::RookValue ; break;
                case Chess::Piece::Queen:  pieceValue = (int) EvulateValue::QueenValue ; break;
                case Chess::Piece::King:   pieceValue = (int) EvulateValue::MateValue ; break; 
            }

            /**
             * merkezdeyse + puan
             */
            if(pieceValue != (int) EvulateValue::MateValue && pieceValue != 0)
            {
                // if(r == e4 e5 d4 d5) 

                if( (r == 4 || r == 5) && ( f == 4 || f == 5) ) pieceValue += 50;
            }

            if (sq.getPieceSide() == Chess::Side::White)
                whiteScore += pieceValue;
            else
                blackScore += pieceValue;
        }
    }

    return whiteScore - blackScore;
}

std::vector<Chess::Move> FlameBoth::Bot::getAllValidMoves(const Chess::Board& board, Chess::Side side)
{
    std::vector<Chess::Move> validMoves;
    validMoves.reserve(64); 

    for (int r1 = 1; r1 <= 8; ++r1)
    {
        for (int f1 = 1; f1 <= 8; ++f1)
        {
            Chess::Square srcSq = board.getSquare({(Chess::File)f1, (Chess::Rank)r1});

            if (srcSq.getPieceSide() == side)
            {
                for (int r2 = 1; r2 <= 8; ++r2)
                {
                    for (int f2 = 1; f2 <= 8; ++f2)
                    {
                        if (r1 == r2 && f1 == f2) continue;

                        Chess::Square destSq = board.getSquare({(Chess::File)f2, (Chess::Rank)r2});

                        if (destSq.getPieceSide() == side) continue;

                        Chess::Move potentialMove(srcSq, destSq);
                        
                        Chess::MoveType type = Chess::MoveValidator(potentialMove, side, board);

                        if (type != Chess::MoveType::Invalid && type != Chess::MoveType::inCheck)
                        {
                            validMoves.push_back(potentialMove);
                        }
                    }
                }
            }
        }
    }
    return validMoves;
}

/**
 * @todo eklenecekler:
 * - vezir çıkma
 * - rok
 * - böyle şeyler işte
 */
int FlameBoth::Bot::fastMoveOrdering(const Chess::Move& move, const Chess::Board& board)
{
    int score = 0;
    
    Chess::Square targetSquare = board.getSquare(move.getTo().getCoordinate());
    Chess::Square sourceSquare = board.getSquare(move.getFrom().getCoordinate());

    if (targetSquare.getPieceType() != Chess::Piece::Empty)
    {
        int victimValue = 0;

        switch(targetSquare.getPieceType())
        {
            case Chess::Piece::Pawn:   victimValue = 100; break;
            case Chess::Piece::Knight: victimValue = 300; break;
            case Chess::Piece::Bishop: victimValue = 300; break;
            case Chess::Piece::Rook:   victimValue = 500; break;
            case Chess::Piece::Queen:  victimValue = 900; break;
            case Chess::Piece::King:   victimValue = 2000; break;
            default: break;
        }

        int aggressorValue = 0; 
        switch(sourceSquare.getPieceType())
        {
            case Chess::Piece::Pawn:   aggressorValue = 100; break;
            case Chess::Piece::Knight: aggressorValue = 300; break;
            case Chess::Piece::Bishop: aggressorValue = 300; break;
            case Chess::Piece::Rook:   aggressorValue = 500; break;
            case Chess::Piece::Queen:  aggressorValue = 900; break;
            case Chess::Piece::King:   aggressorValue = 2000; break;
             default: break;
        }

        score = (victimValue * 10) - aggressorValue;
        
        score += 10000;
    }
    return score;
}