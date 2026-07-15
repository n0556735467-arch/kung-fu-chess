#include <iostream>
#include "src/BoardParser.h"
#include "src/BoardPrinter.h"

int main() {
    try {
        Board board = parseBoard(std::cin);
        printBoard(board, std::cout);
    } catch (const InvalidBoardException& e) {
        // לפי הדרישה - בלי הודעות/debug, אז לא מדפיסים כלום כאן
        return 1;
    }
    return 0;
}