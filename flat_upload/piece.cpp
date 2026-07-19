// placeholder C++ file for Piece (empty)
#include "piece.hpp"

Piece::Piece(int id, Color color, Kind kind, const Position& cell)
    : id(id), color(color), kind(kind), cell(cell), state(PieceState::Idle) {
}