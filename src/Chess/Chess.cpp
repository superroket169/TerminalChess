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

bool Chess::MoveValidator(const Chess::Move& move, const Chess::Side& side, const Chess::Board& board)
{

}

bool Chess::isValidPieceMove(const Move& move, const Board& board)
{
    Square fromSquare = board.getSquare(move.getFrom().getCoordinate());
    Square toSquare = board.getSquare(move.getTo().getCoordinate());

    Piece currentPiece = fromSquare.getPieceType();
    if(currentPiece == Chess::Piece::Empty) return false;

    if(fromSquare.getPieceSide() == toSquare.getPieceSide()) return false;
    if(fromSquare.getCoordinate().file == toSquare.getCoordinate().file && fromSquare.getCoordinate().rank == toSquare.getCoordinate().rank) return false; 

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
            
            if(checkSquare.getPieceType() != Chess::Piece::Empty) return false;
        
            currentFile += fileStep;
            currentRank += rankStep;
        }
    }

    switch (currentPiece)
    {
        case Piece::Pawn    : return isValidPawnMove(move, board);
        case Piece::Bishop  : return isValidBishopMove(move, board);
        case Piece::Knight  : return isValidKnightMove(move, board);
        case Piece::Rook    : return isValidRookMove(move, board);
        case Piece::Queen   : return isValidQueenMove(move, board);
        case Piece::King    : return isValidKingMove(move, board);
    }
    
    return true;
}

bool Chess::isValidPawnMove(const Move& move, const Board& board)
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
            return (toSquare.getPieceType() == Chess::Piece::Empty);
        }
        
        if (diffRank == 2 * direction)
        {
            if (fromRank != startRank) return false;
            return (toSquare.getPieceType() == Chess::Piece::Empty);
        }
    }
    
    else if (diffFile == 1)
    {
        if (diffRank == direction)
        {
            if (toSquare.getPieceType() != Chess::Piece::Empty) return true;

            BoardCoordinate targetCoord = toSquare.getCoordinate();
            BoardCoordinate epTarget = board.getEnPassantTarget();

            if (targetCoord == epTarget) return true;
        }
    }

    return false;
}

bool Chess::isValidKnightMove(const Move& move, const Board& board)
{
    int fromRank = (int)move.getFrom().getCoordinate().rank;
    int fromFile = (int)move.getFrom().getCoordinate().file;
    int toRank   = (int)move.getTo().getCoordinate().rank;
    int toFile   = (int)move.getTo().getCoordinate().file;

    int dRank = std::abs(toRank - fromRank);
    int dFile = std::abs(toFile - fromFile);

    return (dRank == 2 && dFile == 1) || (dRank == 1 && dFile == 2);
}

bool Chess::isValidBishopMove(const Move& move, const Board& board)
{
    int fromRank = (int)move.getFrom().getCoordinate().rank;
    int fromFile = (int)move.getFrom().getCoordinate().file;
    int toRank   = (int)move.getTo().getCoordinate().rank;
    int toFile   = (int)move.getTo().getCoordinate().file;

    int dRank = std::abs(toRank - fromRank);
    int dFile = std::abs(toFile - fromFile);

    return (dRank == dFile);
}

bool Chess::isValidRookMove(const Move& move, const Board& board)
{
    int fromRank = (int)move.getFrom().getCoordinate().rank;
    int fromFile = (int)move.getFrom().getCoordinate().file;
    int toRank   = (int)move.getTo().getCoordinate().rank;
    int toFile   = (int)move.getTo().getCoordinate().file;

    int dRank = std::abs(toRank - fromRank);
    int dFile = std::abs(toFile - fromFile);

    return (dRank == 0 || dFile == 0);
}

bool Chess::isValidQueenMove(const Move& move, const Board& board)
{
    return isValidRookMove(move, board) || isValidBishopMove(move, board);
}

bool Chess::isValidKingMove(const Move&, const Board&)
{
    return true;
}
