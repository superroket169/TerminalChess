#include "FlameBot.h"
#include "../BoardHash/BoardHash.h"
#include "../Time/Time.h"
#include "../Chess/Chess.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <algorithm>

Chess::Move FlameBoth::Bot::getBestMove(Chess::Board board, int depth)
{
    Chess::Move globalBestMove;
    
    std::vector<Chess::Move> moves = getAllValidMoves(board, board.getTurn());
    if (moves.empty()) return globalBestMove;
    
    globalBestMove = moves[0];

    Time timer;
    timer.start();
    double timeLimit = 5.0;

    for(int depthIndex = 1; depthIndex <= depth; depthIndex++)
    {
        Chess::Move currentDepthBestMove;
        bool isWhiteTurn = (board.getTurn() == Chess::Side::White);
        int bestVal = isWhiteTurn ? -999999 : 999999;
        
        // geliştirilmiş ordering alpha-beta cuting hızlanır
        if(depthIndex > 1)
        {
            for(size_t i = 0; i < moves.size(); ++i)
            {
                if(moves[i].getFrom().getCoordinate().file == globalBestMove.getFrom().getCoordinate().file && 
                    moves[i].getFrom().getCoordinate().rank == globalBestMove.getFrom().getCoordinate().rank &&
                    moves[i].getTo().getCoordinate().file == globalBestMove.getTo().getCoordinate().file &&
                    moves[i].getTo().getCoordinate().rank == globalBestMove.getTo().getCoordinate().rank) 
                {
                    std::swap(moves[0], moves[i]);
                    break;
                }
            }
        }


        for(const auto& move : moves)
        {
            Chess::Board tempBoard = board;
            Chess::makeMove(move, board.getTurn(), tempBoard);
            tempBoard.passTurn();
            
            int moveVal = searchTree(tempBoard, depthIndex - 1, -999999, 999999, !isWhiteTurn);
            
            if(isWhiteTurn && (moveVal > bestVal))
            {
                bestVal = moveVal;
                currentDepthBestMove = move;
            }
            else if(moveVal < bestVal)
            {
                bestVal = moveVal;
                currentDepthBestMove = move;
            }
        }

        globalBestMove = currentDepthBestMove;
        
        std::cout << "[Depth] " << depthIndex << " [finished] | [State]: " << bestVal << " | [Timer]: " << timer.elapsedTime() << "s" << "\n";

        if(timer.elapsedTime() > timeLimit)
        {
            std::cout << "time is over. procces stoped\n";
            break; 
        }
    }

    return globalBestMove;
}

/**
 * @bug cant checkmate
 */
int FlameBoth::Bot::searchTree(Chess::Board board, int depth, int alpha, int beta, bool isMaximizing)
{
    std::string boardID = BoardHash::generateID(board);
    if (BoardHash::Table.count(boardID)) 
    {
        BoardHash::Entry& e = BoardHash::Table[boardID];
        if (e.depth >= depth) return e.score;
    }

    if(depth == 0) return evaluate(board);

    Chess::Side currentSide = board.getTurn();
    std::vector<Chess::Move> moves = getAllValidMoves(board, currentSide);

    if(Chess::getGameState(board) == Chess::GameState::Checkmate)
    {
        if(isMaximizing) return -MATE_SCORE - depth;
        else return MATE_SCORE + depth;
    }

    if(moves.empty())
    {
        if (Chess::isKingInCheck(board, currentSide))
        {
            if(isMaximizing) return -MATE_SCORE - depth;
            else return MATE_SCORE + depth;
        }
        else return 0; // Pat
    }

    std::sort(moves.begin(), moves.end(), [&](const Chess::Move& a, const Chess::Move& b){
        return fastMoveOrdering(a, board) > fastMoveOrdering(b, board);
    });

    int maxEval;
    int minEval;

    if (isMaximizing) // White (default)
    {
        maxEval = -INF;
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
    }
    else // Black (default)
    {
        minEval = INF;
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
    }

    int finalScore = isMaximizing ? maxEval : minEval;

    BoardHash::Entry newEntry;
    newEntry.score = finalScore;
    newEntry.depth = depth;
    BoardHash::Table[boardID] = newEntry;

    return finalScore;
}

double specificSigmoid(int totalNonPawnMaterial)
{
    double midPoint = 3500.0; 
    double k = 0.0025; 

    double phase = 1.0 / (1.0 + std::exp(-k * (totalNonPawnMaterial - midPoint)));
    
    return phase;
}

int FlameBoth::Bot::getRawPieceValue(Chess::Piece type)
{
    switch(type)
    {
        case Chess::Piece::Pawn:   return (int) EvulateValue::PawnValue;
        case Chess::Piece::Knight: return (int) EvulateValue::KnightValue;
        case Chess::Piece::Bishop: return (int) EvulateValue::BishopValue;
        case Chess::Piece::Rook:   return (int) EvulateValue::RookValue;
        case Chess::Piece::Queen:  return (int) EvulateValue::QueenValue;
        case Chess::Piece::King:   return (int) EvulateValue::MateValue;
        default: return 0;
    }
}

int FlameBoth::Bot::getMaterialScore(const Chess::Board& board)
{
    int whiteMaterial = 0;
    int blackMaterial = 0;
    int whiteNonPawnMaterial = 0;
    int blackNonPawnMaterial = 0;
    
    int positionalScore = 0;

    for(int r = 1; r <= 8; ++r)
    {
        for (int f = 1; f <= 8; ++f)
        {
            Chess::Square sq = board.getSquare({(Chess::File)f, (Chess::Rank)r});
            if(sq.getPieceType() == Chess::Piece::Empty) continue;

            int val = getRawPieceValue(sq.getPieceType());

            if (sq.getPieceSide() == Chess::Side::White)
            {
                whiteMaterial += val;
                if (sq.getPieceType() != Chess::Piece::Pawn && sq.getPieceType() != Chess::Piece::King) whiteNonPawnMaterial += val;
            }
            else
            {
                blackMaterial += val;
                if (sq.getPieceType() != Chess::Piece::Pawn && sq.getPieceType() != Chess::Piece::King) blackNonPawnMaterial += val;
            }

            if((r == 4 || r == 5) && (f == 4 || f == 5))
            {
                int centerBonus = 0;
                
                if(sq.getPieceType() == Chess::Piece::Pawn) centerBonus = 30;
                else if(sq.getPieceType() == Chess::Piece::Knight) centerBonus = 50;
                else if(sq.getPieceType() == Chess::Piece::Bishop) centerBonus = 40;
                
                if(sq.getPieceSide() == Chess::Side::White) positionalScore += centerBonus;
                else positionalScore -= centerBonus;
            }
        }
    }

    int totalNonPawn = whiteNonPawnMaterial + blackNonPawnMaterial;
    float gameState = specificSigmoid(totalNonPawn);

    int materialScore = whiteMaterial - blackMaterial;
    
    return materialScore + positionalScore;
}

int FlameBoth::Bot::getCastlingScore(const Chess::Board& board)
{
    int castlingScore = 0;
    int castlingScoreWhite = 0;
    int castlingScoreBlack = 0;

    // if(board.isCastled(Side::Black)) 
    // if(board.isCastled(Side::White))


    castlingScore = castlingScoreWhite - castlingScoreBlack;
    return castlingScore;
}

int FlameBoth::Bot::getKingMoveScore(const Chess::Board& board)
{
    int rtrnVal = 0;
    // rtrnVal += 80 * board.isKingMoved(Chess::Side::White) * specificSigmoid(totalNonPawnMaterial);
    // board.isKingMoved(Chess::Side::Black);

    return 0;
}

int FlameBoth::Bot::evaluate(const Chess::Board& board)
{
    int scoreSum = 0;
    scoreSum += getMaterialScore(board);

    return scoreSum;
}

/**
 * @bug cant checkmate
 */
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

