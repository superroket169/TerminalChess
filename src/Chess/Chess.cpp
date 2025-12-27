#include "Chess.h"
#include <cmath>

Chess::BoardCoordinate Chess::Square::getCoordinate() const { return coordinate; }
Chess::Piece Chess::Square::getPieceType() const { return pieceType; }
Chess::Side Chess::Square::getPieceSide() const { return pieceSide; }

Chess::BoardCoordinate Chess::Square::setCoordinate(Chess::BoardCoordinate nw) { return coordinate = nw; }
Chess::Piece Chess::Square::setPieceType(Chess::Piece nw) { return pieceType = nw; }
Chess::Side Chess::Square::setPieceSide(Chess::Side nw) { return pieceSide = nw; }

Chess::Square Chess::Board::getSquare(Chess::BoardCoordinate inp) const { return boardMatris[static_cast<std::size_t>(inp.file)][static_cast<std::size_t>(inp.rank)]; };
Chess::Square Chess::Board::setSquare(Chess::Square inp) { return boardMatris[static_cast<std::size_t>(inp.getCoordinate().file)][static_cast<std::size_t>(inp.getCoordinate().rank)] = inp; };

void Chess::Board::setEnPassantTarget(BoardCoordinate coord) { enPassantTarget = coord; }
void Chess::Board::clearEnPassantTarget() { enPassantTarget = {(File) -1, (Rank)(-1)}; }
Chess::BoardCoordinate Chess::Board::getEnPassantTarget() const { return enPassantTarget; }

Chess::MoveType Chess::MoveValidator(const Chess::Move& move, const Chess::Side& side, const Chess::Board& board)
{
    Chess::MoveType type = isValidPieceMove(move, board);
    if(type == Chess::MoveType::Invalid) return Chess::MoveType::Invalid;


    Square fromSq = board.getSquare(move.getFrom().getCoordinate());
    if(fromSq.getPieceSide() != side) return Chess::MoveType::Invalid;


    Board tempBoard = board;
    Chess::Piece pieceType = fromSq.getPieceType(); 

    Square setSquareFrom = move.getFrom();
    setSquareFrom.setPieceType(Chess::Piece::Empty);
    setSquareFrom.setPieceSide(Chess::Side::None);
    tempBoard.setSquare(setSquareFrom);
    
    Square setSquareTo = move.getTo();
    
    setSquareTo.setPieceType(pieceType); 
    setSquareTo.setPieceSide(side);
    
    tempBoard.setSquare(setSquareTo);

    if(isKingInCheck(tempBoard, side)) return Chess::MoveType::Invalid;

    return type;
}

Chess::MoveType Chess::isValidPieceMove(const Move& move, const Board& board)
{
    Square fromSquare = board.getSquare(move.getFrom().getCoordinate());
    Square toSquare = board.getSquare(move.getTo().getCoordinate());

    Piece currentPiece = fromSquare.getPieceType();
    if(currentPiece == Chess::Piece::Empty) return Chess::MoveType::Invalid;

    if(fromSquare.getPieceSide() == toSquare.getPieceSide()) return Chess::MoveType::Invalid;
    if(fromSquare.getCoordinate().file == toSquare.getCoordinate().file && fromSquare.getCoordinate().rank == toSquare.getCoordinate().rank) return Chess::MoveType::Invalid; 

    /**
     * başlangıç ve bitiş arasında taş varmı bakıyor:
     */
    if(currentPiece != Chess::Piece::Knight)
    {
        int currentFile = (int)fromSquare.getCoordinate().file;
        int currentRank = (int)fromSquare.getCoordinate().rank;
        
        int targetFile = (int)toSquare.getCoordinate().file;
        int targetRank = (int)toSquare.getCoordinate().rank;
    
        int fileStep = 0;
        if (targetFile > currentFile) fileStep = 1;
        else if (targetFile < currentFile) fileStep = -1;
    
        int rankStep = 0;
        if (targetRank > currentRank) rankStep = 1;
        else if (targetRank < currentRank) rankStep = -1;
    
        currentFile += fileStep;
        currentRank += rankStep;
    
        while (currentFile != targetFile || currentRank != targetRank)
        {
            Rank tmpRank = (Rank)currentRank;
            File tmpFile = (File)currentFile;
        
            Square checkSquare = board.getSquare({tmpFile, tmpRank});
            
            if(checkSquare.getPieceType() != Chess::Piece::Empty) return Chess::MoveType::Invalid;
        
            currentFile += fileStep;
            currentRank += rankStep;
        }
    }

    Chess::MoveType currentReturn;

    switch (currentPiece)
    {
        case Piece::Pawn    : currentReturn = isValidPawnMove(move, board);
        case Piece::Bishop  : currentReturn = isValidBishopMove(move, board);
        case Piece::Knight  : currentReturn = isValidKnightMove(move, board);
        case Piece::Rook    : currentReturn = isValidRookMove(move, board);
        case Piece::Queen   : currentReturn = isValidQueenMove(move, board);
        case Piece::King    : currentReturn = isValidKingMove(move, board);
    }
    
    return currentReturn;
}

Chess::MoveType Chess::isValidPawnMove(const Move& move, const Board& board)
{
    Square fromSquare = board.getSquare(move.getFrom().getCoordinate());
    Square toSquare = board.getSquare(move.getTo().getCoordinate());

    int direction = (fromSquare.getPieceSide() == Chess::Side::White) ? 1 : -1;

    int startRank = (fromSquare.getPieceSide() == Chess::Side::White) ? 2 : 7;

    int fromRank = (int)fromSquare.getCoordinate().rank;
    int fromFile = (int)fromSquare.getCoordinate().file;
    
    int toRank = (int)toSquare.getCoordinate().rank;
    int toFile = (int)toSquare.getCoordinate().file;

    int diffFile = std::abs(toFile - fromFile);
    int diffRank = toRank - fromRank;

    if (diffFile == 0)
    {
        if (diffRank == direction)
        {
            return (toSquare.getPieceType() == Chess::Piece::Empty) ? Chess::MoveType::Valid : Chess::MoveType::Invalid;
        }
        
        if (diffRank == 2 * direction)
        {
            if (fromRank != startRank) return Chess::MoveType::Invalid;
            return (toSquare.getPieceType() == Chess::Piece::Empty) ? Chess::MoveType::DoublePush : Chess::MoveType::Invalid;
        }
    }
    
    else if (diffFile == 1)
    {
        if (diffRank == direction)
        {
            if (toSquare.getPieceType() != Chess::Piece::Empty) return Chess::MoveType::PawnCapture;

            BoardCoordinate targetCoord = toSquare.getCoordinate();
            BoardCoordinate epTarget = board.getEnPassantTarget();

            if (targetCoord == epTarget) return Chess::MoveType::EnPassant;
        }
    }

    return Chess::MoveType::Invalid;
}

Chess::MoveType Chess::isValidKnightMove(const Move& move, const Board& board)
{
    int fromRank = (int)move.getFrom().getCoordinate().rank;
    int fromFile = (int)move.getFrom().getCoordinate().file;
    int toRank   = (int)move.getTo().getCoordinate().rank;
    int toFile   = (int)move.getTo().getCoordinate().file;

    int dRank = std::abs(toRank - fromRank);
    int dFile = std::abs(toFile - fromFile);

    bool currentReturn = (dRank == 2 && dFile == 1) || (dRank == 1 && dFile == 2);
    if(!currentReturn) return MoveType::Invalid;

    if(move.getTo().getPieceType() != Chess::Piece::Empty) return MoveType::Capture;
    return MoveType::Valid;
}

Chess::MoveType Chess::isValidBishopMove(const Move& move, const Board& board)
{
    int fromRank = (int)move.getFrom().getCoordinate().rank;
    int fromFile = (int)move.getFrom().getCoordinate().file;
    int toRank   = (int)move.getTo().getCoordinate().rank;
    int toFile   = (int)move.getTo().getCoordinate().file;

    int dRank = std::abs(toRank - fromRank);
    int dFile = std::abs(toFile - fromFile);

    bool currentReturn = (dRank == dFile);
    if(!currentReturn) return MoveType::Invalid;

    if(move.getTo().getPieceType() != Chess::Piece::Empty) return MoveType::Capture;
    return MoveType::Valid;
}

Chess::MoveType Chess::isValidRookMove(const Move& move, const Board& board)
{
    int fromRank = (int)move.getFrom().getCoordinate().rank;
    int fromFile = (int)move.getFrom().getCoordinate().file;
    int toRank   = (int)move.getTo().getCoordinate().rank;
    int toFile   = (int)move.getTo().getCoordinate().file;

    int dRank = std::abs(toRank - fromRank);
    int dFile = std::abs(toFile - fromFile);

    bool currentReturn = (dRank == 0 || dFile == 0);
    if(!currentReturn) return MoveType::Invalid;

    if(move.getTo().getPieceType() != Chess::Piece::Empty) return MoveType::Capture;
    return MoveType::Valid;
}

Chess::MoveType Chess::isValidQueenMove(const Move& move, const Board& board)
{
    bool currentReturn = (bool)isValidRookMove(move, board) || (bool)isValidBishopMove(move, board);
    if(!currentReturn) return MoveType::Invalid;

    if(move.getTo().getPieceType() != Chess::Piece::Empty) return MoveType::Capture;
    return MoveType::Valid;
}

Chess::MoveType Chess::isValidKingMove(const Move&, const Board&)
{
    return Chess::MoveType::Valid;
    /**
     * other return positions: 
     * - Chess::MoveType::KingsideCastling;
     * - Chess::MoveType::QueensideCastling
     */
}

bool Chess::isKingInCheck(const Board&, Side)
{
    return true;
}

bool Chess::isAttackedBy(Square /**attacker square */, Square /** king square */, const Board&)
{
    return true;
}
