#include <iostream>
#include <sstream>
#include <string>
#include "src/BoardParser.h"
#include "src/BoardPrinter.h"

int main() {
    std::string line;

    std::getline(std::cin, line);  

    std::ostringstream boardText;
    while (std::getline(std::cin, line)) {
        if (line == "Commands:") {
            break;
        }
        boardText << line << "\n";
    }

    try {
        std::istringstream boardInput(boardText.str());
        Board board = parseBoard(boardInput);

        while (std::getline(std::cin, line)) {
            if (line == "print board") {
                printBoard(board, std::cout);
            }
        }
    } catch (const InvalidBoardException& e) {
        if (e.type == BoardErrorType::UnknownToken) {
            std::cout << "ERROR UNKNOWN_TOKEN" << std::endl;
        } else {
            std::cout << "ERROR ROW_WIDTH_MISMATCH" << std::endl;
        }
    }

    return 0;
}