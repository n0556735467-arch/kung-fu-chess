// placeholder header for board parser
#ifndef BOARD_PARSER_HPP
#define BOARD_PARSER_HPP
#include <istream>
#include <stdexcept>
#include "board.hpp"

enum class BoardErrorType { UnknownToken, RowWidthMismatch };

class InvalidBoardException : public std::runtime_error {
public:
    BoardErrorType type;
    InvalidBoardException(BoardErrorType type, const std::string& msg);
};

Piece parseToken(const std::string& token, int row, int col, int id);
Board parseBoard(std::istream& input);

#endif