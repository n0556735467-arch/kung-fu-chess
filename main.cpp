// placeholder main app file (empty)
#include <iostream>
#include "io/board_parser.hpp"
#include "io/board_printer.hpp"

int main() {
    try {
        Board board = parseBoard(std::cin);
        printBoard(board, std::cout);
    } catch (const InvalidBoardException&) {
        return 1;
    }
    return 0;
}