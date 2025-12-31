#include "BoardHash.h"

std::string BoardHash::generateID(const Chess::Board& board)
{
    // 64 Kare + 4 + 2  + 1 + 2 = 73
    std::string hashString(73, ' '); 

    for(int i = 1; i <= 8 ; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            int index = (i - 1) * 8 + (j - 1);
            
            Chess::Square sq = board.getSquare({(Chess::File)i, (Chess::Rank)j});
            char pieceChar = '.';

            switch(sq.getPieceType())
            {
                case Chess::Piece::Bishop : pieceChar = 'B'; break;
                case Chess::Piece::King :   pieceChar = 'K'; break;
                case Chess::Piece::Knight : pieceChar = 'N'; break;
                case Chess::Piece::Pawn :   pieceChar = 'P'; break;
                case Chess::Piece::Queen :  pieceChar = 'Q'; break;
                case Chess::Piece::Rook :   pieceChar = 'R'; break;
                case Chess::Piece::Empty :  pieceChar = '.'; break;
            }

            if(sq.getPieceType() != Chess::Piece::Empty) if(sq.getPieceSide() == Chess::Side::Black) pieceChar += 32;
            
            hashString[index] = pieceChar;
        }
    }

    int k = 64; 

    hashString[k++] = board.isKingMoved(Chess::Side::White) ? '1' : '0';
    hashString[k++] = board.isKingMoved(Chess::Side::Black) ? '1' : '0';

    hashString[k++] = board.isKingRookMoved(Chess::Side::White) ? '1' : '0';
    hashString[k++] = board.isQueenRookMoved(Chess::Side::White) ? '1' : '0';
    hashString[k++] = board.isKingRookMoved(Chess::Side::Black) ? '1' : '0';
    hashString[k++] = board.isQueenRookMoved(Chess::Side::Black) ? '1' : '0';

    hashString[k++] = (board.getTurn() == Chess::Side::White ? 'W' : 'B');

    hashString[k++] = (char)board.getEnPassantTarget().file; 
    hashString[k++] = (char)board.getEnPassantTarget().rank;

    return hashString;
}
