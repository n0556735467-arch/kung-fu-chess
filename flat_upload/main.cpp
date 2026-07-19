#include <iostream>
#include <sstream>
#include <string>
#include "board_parser.hpp"
#include "board_printer.hpp"
#include "game_engine.hpp"
#include "controller.hpp"

int main() {
    std::string line;

    std::getline(std::cin, line);  // "Board:" - מדלגים

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
        GameEngine engine(board);
        Controller controller(engine);

        while (std::getline(std::cin, line)) {
            std::istringstream cmd(line);
            std::string action;
            cmd >> action;

            if (action == "click") {
                int x, y;
                cmd >> x >> y;
                controller.click(x, y);
            } else if (action == "wait") {
                int ms;
                cmd >> ms;
                engine.wait(ms);
            } else if (action == "print") {
                std::string what;
                cmd >> what;
                if (what == "board") {
                    printBoard(engine.getBoard(), std::cout);
                }
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