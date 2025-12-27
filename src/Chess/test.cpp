#include <iostream>
#include <cctype> // toupper icin
#include "Chess.h"

// Tas tipine ve rengine gore ekrana basilacak karakteri secer
char getPieceChar(Chess::Piece p, Chess::Side s)
{
    char c = '.'; // Bos kare

    switch (p)
    {
        case Chess::Piece::Pawn:   c = 'p'; break;
        case Chess::Piece::Rook:   c = 'r'; break;
        case Chess::Piece::Knight: c = 'n'; break; // Knight genelde 'N' ile gosterilir ama 'n' yaptim
        case Chess::Piece::Bishop: c = 'b'; break;
        case Chess::Piece::Queen:  c = 'q'; break;
        case Chess::Piece::King:   c = 'k'; break;
        case Chess::Piece::Empty:  return '.';
        default: return '?';
    }

    // Beyaz taslari Buyuk Harf yapalim ki yonu belli olsun (Opsiyonel)
    if (s == Chess::Side::White)
    {
        c = std::toupper(c);
    }

    return c;
}

int main()
{
    Chess::Board board;

    std::cout << "   1 2 3 4 5 6 7 8  (Rank)" << std::endl;
    std::cout << "  ----------------" << std::endl;

    // Dis dongu File (A-H) -> Bizim matrisin satir yapisi
    for (int i = 1; i <= 8; ++i)
    {
        auto file = static_cast<Chess::File>(i);
        char fileChar = 'A' + (i - 1);
        
        std::cout << fileChar << "| ";

        // Ic dongu Rank (1-8) -> Bizim matrisin sutun yapisi
        for (int j = 1; j <= 8; ++j)
        {
            auto rank = static_cast<Chess::Rank>(j);
            
            // Koordinati olustur ve tasi getir
            Chess::BoardCoordinate coord(file, rank);
            Chess::Square sq = board.getSquare(coord);

            // Ekrana yazdir
            std::cout << getPieceChar(sq.getPieceType(), sq.getPieceSide()) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}