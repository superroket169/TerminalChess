#include "BoardHash.h"
#include <random>
#include <cstring>
#include <cctype>

std::string BoardHash::generateID(const Chess::Board& board)
{
    // 64 Kare + 4 + 2  + 1 + 2 + 2 = 73
    std::string hashString(75, ' '); 

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

    hashString[k++] = (char)board.isCastled(Chess::Side::White);
    hashString[k++] = (char)board.isCastled(Chess::Side::Black);

    return hashString;
}

Chess::Board BoardHash::loadID(const std::string& id)
{
    Chess::Board board; 

    for(int i = 1; i <= 8; ++i) 
    {
        for(int j = 1; j <= 8; ++j) 
        {
            int index = (i - 1) * 8 + (j - 1);
            char pChar = id[index];

            Chess::Square sq = board.getSquare({(Chess::File)i, (Chess::Rank)j});
            
            Chess::Piece type = Chess::Piece::Empty;
            Chess::Side side = Chess::Side::None;

            if (pChar != '.')
            {
                if (std::islower(pChar))
                {
                    side = Chess::Side::Black;
                    pChar = std::toupper(pChar);
                }
                else
                {
                    side = Chess::Side::White;
                }

                switch(pChar)
                {
                    case 'P': type = Chess::Piece::Pawn; break;
                    case 'N': type = Chess::Piece::Knight; break;
                    case 'B': type = Chess::Piece::Bishop; break;
                    case 'R': type = Chess::Piece::Rook; break;
                    case 'Q': type = Chess::Piece::Queen; break;
                    case 'K': type = Chess::Piece::King; break;
                    default:  type = Chess::Piece::Empty; break;
                }
            }

            sq.setPieceType(type);
            sq.setPieceSide(side);
            
            board.setSquare(sq);
        }
    }

    int k = 64; 

    board.setKingMoved(Chess::Side::White, (id[k++] == '1'));
    board.setKingMoved(Chess::Side::Black, (id[k++] == '1'));

    board.setKingRookMoved(Chess::Side::White, (id[k++] == '1'));
    board.setQueenRookMoved(Chess::Side::White, (id[k++] == '1'));
    board.setKingRookMoved(Chess::Side::Black, (id[k++] == '1'));
    board.setQueenRookMoved(Chess::Side::Black, (id[k++] == '1'));

    char turnChar = id[k++];
    Chess::Side desiredTurn = (turnChar == 'W' ? Chess::Side::White : Chess::Side::Black);
    
    if (board.getTurn() != desiredTurn)
    {
        board.passTurn();
    }

    char epFile = id[k++];
    char epRank = id[k++];
    board.setEnPassantTarget({(Chess::File)epFile, (Chess::Rank)epRank});

    board.setCastled(Chess::Side::White, (bool)id[k++]); 
    board.setCastled(Chess::Side::Black, (bool)id[k++]); 

    return board;
}

/*
namespace BoardHash
{
    // not : rastgele sayı üretiminde seed sabittir
    // amaç ileride maç pozisyonlarını ai eğitmek için kullanmak ve opening'leri optimize etmektir

    // [64 Kare] [12 Taş Tipi] (6 W + 6 B)
    Key PieceKeys[64][12];
    Key CastleKeys[16];
    Key EnPassantKeys[9];
    Key SideKey;

    Entry* Table = nullptr;

    /**
     * @brief tohumu başlatma işlemlerini yapar:
     * - not: kod tam olarak bana ait değil ai dan destek aldım: 
     * - sadece init de ve veri türlerinde & rastgeleleştirmede (orn : mt19937_64)
    
    void init()
    {
        // sabit seed (Tohum):
        std::mt19937_64 rng(123456789ULL);
        std::uniform_int_distribution<uint64_t> dist;

        for (int sq = 0; sq < 64; ++sq)
            for (int p = 0; p < 12; ++p)
                PieceKeys[sq][p] = dist(rng);

        for (int i = 0; i < 16; ++i)
            CastleKeys[i] = dist(rng);

        for (int i = 0; i < 9; ++i) 
            EnPassantKeys[i] = dist(rng);

        SideKey = dist(rng);

        if (Table == nullptr)
            Table = new Entry[TABLE_SIZE];
        clear();
    }

    void clear()
    {
        if (Table != nullptr) 
            std::memset(Table, 0, sizeof(Entry) * TABLE_SIZE);
    }

    Key generateHash(const Chess::Board& board)
    {
        Key finalHash = 0;

        for (int file = 1; file <= 8; ++file) 
        {
            for (int rank = 1; rank <= 8; ++rank) 
            {
                Chess::Square sq = board.getSquare({(Chess::File)file, (Chess::Rank)rank});

                if (sq.getPieceType() != Chess::Piece::Empty)
                {
                    // W: P=0, N=1, B=2, R=3, Q=4, K=5
                    // B: P=6, N=7, B=8, R=9, Q=10, K=11
                    int pieceIndex = 0;

                    switch (sq.getPieceType()) {
                        case Chess::Piece::Pawn:   pieceIndex = 0; break;
                        case Chess::Piece::Knight: pieceIndex = 1; break;
                        case Chess::Piece::Bishop: pieceIndex = 2; break;
                        case Chess::Piece::Rook:   pieceIndex = 3; break;
                        case Chess::Piece::Queen:  pieceIndex = 4; break;
                        case Chess::Piece::King:   pieceIndex = 5; break;
                        default: break;
                    }

                    if (sq.getPieceSide() == Chess::Side::Black) pieceIndex += 6;

                    int squareIndex = (rank - 1) * 8 + (file - 1);

                    finalHash ^= PieceKeys[squareIndex][pieceIndex];
                }
            }
        }

        // Bit 0: KingSide, Bit 1: QueenSide
        int castleMask = 0;
        if (board.isKingRookMoved(Chess::Side::White))  castleMask |= 1;
        if (board.isQueenRookMoved(Chess::Side::White)) castleMask |= 2;
        if (board.isKingRookMoved(Chess::Side::Black))  castleMask |= 4;
        if (board.isQueenRookMoved(Chess::Side::Black)) castleMask |= 8;
        
        finalHash ^= CastleKeys[castleMask];

        int epFile = (int)board.getEnPassantTarget().file; // 0 , 1-8
        if (epFile > 0 && epFile <= 8)
            finalHash ^= EnPassantKeys[epFile];

        if (board.getTurn() == Chess::Side::Black)
            finalHash ^= SideKey;

        return finalHash;
    }
}

*/
