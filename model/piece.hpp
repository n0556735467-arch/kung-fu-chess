// placeholder header for Piece
#ifndef PIECE_HPP
#define PIECE_HPP
#include "position.hpp"

enum class Color { White, Black };
enum class Kind  { King, Queen, Rook, Bishop, kNight, Pawn };

struct Piece {
    int id;
    Color color;
    Kind kind;
    Position cell;

    Piece(int id, Color color, Kind kind, Position cell);
};

#endif