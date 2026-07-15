#ifndef BOARD_PRINTER_H
#define BOARD_PRINTER_H
#include <ostream>
#include <vector>
#include <string>
#include "Board.h"

inline char colorChar(Color color) {
    return color == Color::White ? 'w' : 'b';
}

inline char kindChar(Kind kind) {
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

inline void printBoard(const Board& board, std::ostream& out) {
    // בונים מטריצה של טוקנים, כולה מאותחלת ל-"." (תא ריק)
    std::vector<std::vector<std::string>> grid(
        board.rows, std::vector<std::string>(board.cols, ".")
    );

    // ממלאים בתוך המטריצה כל כלי שיש ב-board.pieces
    for (const Piece& p : board.pieces) {
        std::string token;
        token += colorChar(p.color);
        token += kindChar(p.kind);
        grid[p.cell.row][p.cell.col] = token;
    }

    // מדפיסים שורה-שורה, טוקנים מופרדים ברווח
    for (int r = 0; r < board.rows; r++) {
        for (int c = 0; c < board.cols; c++) {
            if (c > 0) {
                out << " ";
            }
            out << grid[r][c];
        }
        out << "\n";
    }
}

#endif