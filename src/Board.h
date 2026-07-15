#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include "Piece.h"

struct Board {
    int rows;
    int cols;
    std::vector<Piece> pieces;
    
    Board(int rows, int cols, std::vector<Piece> pieces)
        : rows(rows), cols(cols), pieces(pieces) {
    }
};

#endif