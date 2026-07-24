#ifndef PIECE_HPP
#define PIECE_HPP
#include "position.hpp"

enum class Color { White, Black };
enum class Kind  { King, Queen, Rook, Bishop, Knight, Pawn };
enum class PieceState { Idle, Moving, Airborne, Captured };

struct Piece {
    int id;
    Color color;
    Kind kind;
    Position cell;
    PieceState state;

    Piece(int id, Color color, Kind kind, const Position& cell);
};

#endif