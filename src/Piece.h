#ifndef PIECE_H
#define PIECE_H
#include "Position.h"

enum class Color { White, Black }; 
enum class Kind  { King, Queen, Rook, Bishop, kNight, Pawn }; 

struct Piece {
    Color color;
    Kind kind;
    Position cell;
    
    Piece(Color color, Kind kind, Position cell)
        : color(color), kind(kind), cell(cell) {
    }
};

#endif