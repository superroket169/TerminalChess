#include "Chess.h"

Chess::BoardCoordinate Chess::Square::getCoordinate() const { return coordinate; }
Chess::Piece Chess::Square::getPieceType() const { return pieceType; }
Chess::Side Chess::Square::getPieceSide() const { return pieceSide; }

Chess::BoardCoordinate Chess::Square::setCoordinate(Chess::BoardCoordinate nw) { return coordinate = nw; }
Chess::Piece Chess::Square::setPieceType(Chess::Piece nw) { return pieceType = nw; }
Chess::Side Chess::Square::setPieceSide(Chess::Side nw) { return pieceSide = nw; }

Chess::Square Chess::Board::getSquare(Chess::BoardCoordinate inp) const { return boardMatris[static_cast<std::size_t>(inp.file)][static_cast<std::size_t>(inp.rank)]; };
Chess::Square Chess::Board::setSquare(Chess::Square inp) { return boardMatris[static_cast<std::size_t>(inp.getCoordinate().file)][static_cast<std::size_t>(inp.getCoordinate().rank)] = inp; };
