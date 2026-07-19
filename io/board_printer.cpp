// placeholder C++ file for board printer (empty)
#include <vector>
#include <string>
#include "board_printer.hpp"

static char colorChar(Color color) {
    return color == Color::White ? 'w' : 'b';
}

static char kindChar(Kind kind) {
    switch (kind) {
        case Kind::King:   return 'K';
        case Kind::Queen:  return 'Q';
        case Kind::Rook:   return 'R';
        case Kind::Bishop: return 'B';
        case Kind::kNight: return 'N';
        case Kind::Pawn:   return 'P';
    }
    return '?';
}

void printBoard(const Board& board, std::ostream& out) {
    std::vector<std::vector<std::string>> grid(
        board.rows, std::vector<std::string>(board.cols, ".")
    );

    for (const Piece& p : board.getPieces()) {
        std::string token;
        token += colorChar(p.color);
        token += kindChar(p.kind);
        grid[p.cell.row][p.cell.col] = token;
    }

    for (int r = 0; r < board.rows; r++) {
        for (int c = 0; c < board.cols; c++) {
            if (c > 0) out << " ";
            out << grid[r][c];
        }
        out << "\n";
    }
}