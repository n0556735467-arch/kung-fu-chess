// placeholder header for Piece
#ifndef PIECE_HPP
#define PIECE_HPP
#include "position.hpp"

enum class Color { White, Black };
enum class Kind  { King, Queen, Rook, Bishop, kNight, Pawn };
enum class PieceState { Idle, Moving, Captured };

struct Piece {
    int id;
    Color color;
    Kind kind;
    Position cell;
    PieceState state;

    Piece(int id, Color color, Kind kind, Position cell);
};

#endif