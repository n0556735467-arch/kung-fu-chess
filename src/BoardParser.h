#ifndef BOARD_PARSER_H
#define BOARD_PARSER_H
#include <sstream>
#include <vector>
#include <stdexcept>
#include "Board.h"

class InvalidBoardException : public std::runtime_error {
public:
    InvalidBoardException(const std::string& msg) : std::runtime_error(msg) {}
};

inline Piece parseToken(const std::string& token, int row, int col) {
    if (token.size() != 2) {
        throw InvalidBoardException("Invalid piece token: " + token);
    }

    Color color;
    if (token[0] == 'w') {
        color = Color::White;
    } else if (token[0] == 'b') {
        color = Color::Black;
    } else {
        throw InvalidBoardException("Invalid color letter in token: " + token);
    }

    Kind kind;
    switch (token[1]) {
        case 'K': kind = Kind::King;   break;
        case 'Q': kind = Kind::Queen;  break;
        case 'R': kind = Kind::Rook;   break;
        case 'B': kind = Kind::Bishop; break;
        case 'N': kind = Kind::kNight; break;
        case 'P': kind = Kind::Pawn;   break;
        default:
            throw InvalidBoardException("Invalid kind letter in token: " + token);
    }

    return Piece(color, kind, Position(row, col));
}

inline Board parseBoard(std::istream& input) {
    std::vector<Piece> allPieces;
    std::string line;
    int rowIndex = 0;
    int expectedCols = -1;

    while (std::getline(input, line)) {
        std::istringstream rowStream(line);
        std::string token;
        int col = 0;

        while (rowStream >> token) {
            if (token != ".") {
                allPieces.push_back(parseToken(token, rowIndex, col));
            }
            col++;
        }

        if (expectedCols == -1) {
            expectedCols = col;
        } else if (col != expectedCols) {
            throw InvalidBoardException("Inconsistent row length");
        }

        rowIndex++;
    }

    return Board(rowIndex, expectedCols, allPieces);
}

#endif