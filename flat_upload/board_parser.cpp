// placeholder C++ file for board parser (empty)
#include <sstream>
#include "board_parser.hpp"

InvalidBoardException::InvalidBoardException(BoardErrorType type, const std::string& msg)
    : std::runtime_error(msg), type(type) {
}

Piece parseToken(const std::string& token, int row, int col, int id) {
    if (token.size() != 2) {
        throw InvalidBoardException(BoardErrorType::UnknownToken, "Invalid piece token: " + token);
    }

    Color color;
    if (token[0] == 'w') {
        color = Color::White;
    } else if (token[0] == 'b') {
        color = Color::Black;
    } else {
        throw InvalidBoardException(BoardErrorType::UnknownToken, "Invalid color letter in token: " + token);
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
            throw InvalidBoardException(BoardErrorType::UnknownToken, "Invalid kind letter in token: " + token);
    }

    return Piece(id, color, kind, Position(row, col));
}

Board parseBoard(std::istream& input) {
    std::vector<std::string> lines;
    std::string line;
    int expectedCols = -1;

    while (std::getline(input, line)) {
        std::istringstream rowStream(line);
        std::string token;
        int col = 0;
        while (rowStream >> token) {
            col++;
        }
        if (expectedCols == -1) {
            expectedCols = col;
        } else if (col != expectedCols) {
            throw InvalidBoardException(BoardErrorType::RowWidthMismatch, "Inconsistent row length");
        }
        lines.push_back(line);
    }

    Board board(static_cast<int>(lines.size()), expectedCols);
    int nextId = 0;

    for (int row = 0; row < static_cast<int>(lines.size()); row++) {
        std::istringstream rowStream(lines[row]);
        std::string token;
        int col = 0;
        while (rowStream >> token) {
            if (token != ".") {
                board.addPiece(parseToken(token, row, col, nextId));
                nextId++;
            }
            col++;
        }
    }

    return board;
}