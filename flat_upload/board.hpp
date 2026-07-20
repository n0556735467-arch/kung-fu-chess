// placeholder header for Board
#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
#include "piece.hpp"

class Board {
public:
    Board(int rows, int cols);

    int rows;
    int cols;

    bool isInBounds(Position pos) const;
    const Piece* pieceAt(Position pos) const;
    Piece* findPieceById(int id);
    void addPiece(Piece piece);
    void removePiece(Position pos);
    void movePiece(Position from, Position to);
    const std::vector<Piece>& getPieces() const;

private:
    std::vector<Piece> pieces;
};

#endif