// placeholder C++ file for Board (empty)
#include <stdexcept>
#include <utility>
#include "board.hpp"

Board::Board(int rows, int cols) : rows(rows), cols(cols) {
}

bool Board::isInBounds(Position pos) const {
    return pos.row >= 0 && pos.row < rows && pos.col >= 0 && pos.col < cols;
}

const Piece* Board::pieceAt(Position pos) const {
    for (const Piece& p : pieces) {
        if (p.cell == pos) {
            return &p;
        }
    }
    return nullptr;
}

void Board::addPiece(Piece piece) {
    if (pieceAt(piece.cell) != nullptr) {
        throw std::runtime_error("Cell already occupied");
    }
    pieces.push_back(std::move(piece));
}

void Board::removePiece(Position pos) {
    for (size_t i = 0; i < pieces.size(); i++) {
        if (pieces[i].cell == pos) {
            pieces.erase(pieces.begin() + i);
            return;
        }
    }
}

void Board::movePiece(Position from, Position to) {
    for (Piece& p : pieces) {
        if (p.cell == from) {
            p.cell = to;
            return;
        }
    }
}

const std::vector<Piece>& Board::getPieces() const {
    return pieces;
}