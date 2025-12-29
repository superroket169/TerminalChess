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
        int currentargetFile = (int)fromSquare.getCoordinate().file;
        int currentargetRank = (int)fromSquare.getCoordinate().rank;
        
        int targetargetFile = (int)toSquare.getCoordinate().file;
        int targetargetRank = (int)toSquare.getCoordinate().rank;
    
        int fileStep = 0;
        if (targetargetFile > currentargetFile) fileStep = 1;
        else if (targetargetFile < currentargetFile) fileStep = -1;
    
        int rankStep = 0;
        if (targetargetRank > currentargetRank) rankStep = 1;
        else if (targetargetRank < currentargetRank) rankStep = -1;
    
        currentargetFile += fileStep;
        currentargetRank += rankStep;
    
        while (currentargetFile != targetargetFile || currentargetRank != targetargetRank)
        {
            Rank tmpRank = (Rank)currentargetRank;
            File tmpFile = (File)currentargetFile;
        
            Square checkSquare = board.getSquare({tmpFile, tmpRank});
            
            if(checkSquare.getPieceType() != Chess::Piece::Empty) return Chess::MoveType::Invalid;
        
            currentargetFile += fileStep;
            currentargetRank += rankStep;
        }
    }

    Chess::MoveType currentReturn;

    switch (currentPiece)
    {
        case Piece::Pawn    : return isValidPawnMove(move, board);
        case Piece::Bishop  : return isValidBishopMove(move, board);
        case Piece::Knight  : return isValidKnightMove(move, board);
        case Piece::Rook    : return isValidRookMove(move, board);
        case Piece::Queen   : return isValidQueenMove(move, board);
        case Piece::King    : return isValidKingMove(move, board);
    }

    return Chess::MoveType::Invalid;
}

Chess::MoveType Chess::isValidPawnMove(const Move& move, const Board& board)
{
    Square fromSquare = board.getSquare(move.getFrom().getCoordinate());
    Square toSquare = board.getSquare(move.getTo().getCoordinate());

    int direction = (fromSquare.getPieceSide() == Chess::Side::White) ? 1 : -1;

    int startargetRank = (fromSquare.getPieceSide() == Chess::Side::White) ? 2 : 7;

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
            if (fromRank != startargetRank) return Chess::MoveType::Invalid;
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
    MoveType rookMove = isValidRookMove(move, board);
    MoveType bishopMove = isValidBishopMove(move, board);

    if (rookMove != MoveType::Invalid) return rookMove;
    
    if (bishopMove != MoveType::Invalid) return bishopMove;

    return MoveType::Invalid;
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

/**
 * yapılacaklar : 
 * - şahın konumunu bul (side a göre)
 * - tüm tahtadaki herhangi bir taşın şaha gidebilip gidemediğini kontrol et
 * - - gidebiliyorsa return true
 * - - yoksa false
 */
bool Chess::isKingInCheck(const Board& board, Side side)
{
    BoardCoordinate kingPos;
    bool found = false;

    // şahı bulma:
    for (int r = 1; r <= 8; ++r)
    {
        for (int f = 1; f <= 8; ++f)
        {
            Square sq = board.getSquare({(File)f, (Rank)r});
            if (sq.getPieceType() == Piece::King && sq.getPieceSide() == side)
            {
                kingPos = {(File)f, (Rank)r};
                found = true;
                break;
            }
        }
        if (found) break;
    }

    // şah yoksa false (testler için).
    if (!found) return false;

    Side enemySide = (side == Side::White) ? Side::Black : Side::White;
    return isSquareAttacked(kingPos, enemySide, board);
}
bool Chess::isAttackedBy(Square /**attacker tmpSquareuare */, Square /** king tmpSquareuare */, const Board&)
{
    return false;
}

bool Chess::isSquareAttacked(BoardCoordinate target, Side attackerSide, const Board& board)
{
    int targetFile = static_cast<int>(target.file);
    int targetRank = static_cast<int>(target.rank);

    /**
     * Kale ve Vezir kontrolü:
     */
    int straightDirs[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    for (auto& dir : straightDirs)
    {
        for (int dist = 1; dist < 8; ++dist)
        {
            // boardın içinde mi kontrolü
            int checkFile = targetFile + (dir[0] * dist);
            int checkRank = targetRank + (dir[1] * dist);
            if (checkFile < 1 || checkFile > 8 || checkRank < 1 || checkRank > 8) break;            

            Square tmpSquare = board.getSquare({(File)checkFile, (Rank)checkRank});
            Piece pieceType = tmpSquare.getPieceType();
            Side pieceSide = tmpSquare.getPieceSide();

            if (pieceType != Piece::Empty)
            {
                if (pieceSide == attackerSide && (pieceType == Piece::Rook || pieceType == Piece::Queen))
                    return true;
                break; // daha fazla taramaya gereke yok
            }
        }
    }

    /**
     * Vezir ve Fil kontrolü
     */
    int diagDirs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

    for (auto& dir : diagDirs)
    {
        for (int dist = 1; dist < 8; ++dist)
        {
            int checkFile = targetFile + (dir[0] * dist);
            int checkRank = targetRank + (dir[1] * dist);

            if (checkFile < 1 || checkFile > 8 || checkRank < 1 || checkRank > 8) break;

            Square tmpSquare = board.getSquare({(File)checkFile, (Rank)checkRank});
            Piece pieceType = tmpSquare.getPieceType();
            Side pieceSide = tmpSquare.getPieceSide();

            if (pieceType != Piece::Empty)
            {
                if (pieceSide == attackerSide
                && (pieceType == Piece::Bishop || pieceType == Piece::Queen))
                    return true;
                
                break; // daha fazla taramaya gereke yok
            }
        }
    }

    /**
     * at kontrolü
     */
    int knightMoves[8][2] = { 
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1} 
    };

    for (auto& move : knightMoves)
    {
        int checkFile = targetFile + move[0];
        int checkRank = targetRank + move[1];

        if (checkFile >= 1 && checkFile <= 8 && checkRank >= 1 && checkRank <= 8)
        {
            Square tmpSquare = board.getSquare({(File)checkFile, (Rank)checkRank});

            if (tmpSquare.getPieceSide() == attackerSide
            && tmpSquare.getPieceType() == Piece::Knight)
                return true;
            
        }
    }

    /**
     * piyon kontrolü
     */
    int pawnAttackRank = (attackerSide == Side::White) ? targetRank - 1 : targetRank + 1;

    if (pawnAttackRank >= 1 && pawnAttackRank <= 8)
    {
        int attackFiles[] = { targetFile - 1, targetFile + 1 };

        for (int i : attackFiles)
        {
            if (i >= 1 && i <= 8)
            {
                Square tmpSquare = board.getSquare({(File)i, (Rank)pawnAttackRank});

                if (tmpSquare.getPieceSide() == attackerSide
                && tmpSquare.getPieceType() == Piece::Pawn)
                    return true;
            }
        }
    }

    /**
     * şah kontrolü birbirlerine yaklaşmasınlar diye
     */
    int kingMoves[8][2] = { 
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} 
    };

    for (auto& move : kingMoves)
    {
        int checkFile = targetFile + move[0];
        int checkRank = targetRank + move[1];

        if (checkFile >= 1 && checkFile <= 8 && checkRank >= 1 && checkRank <= 8)
        {
            Square tmpSquare = board.getSquare({(File)checkFile, (Rank)checkRank});

            if (tmpSquare.getPieceSide() == attackerSide
            && tmpSquare.getPieceType() == Piece::King)
                return true;
        }
    }

    return false;
}