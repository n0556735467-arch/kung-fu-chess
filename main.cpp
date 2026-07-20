#include <iostream>
#include <sstream>
#include <string>
#include "io/board_parser.hpp"
#include "io/board_printer.hpp"
#include "engine/game_engine.hpp"
#include "input/controller.hpp"
#include "input/board_mapper.hpp"

int main() {
    std::string line;
    std::getline(std::cin, line);

    std::ostringstream boardText;
    while (std::getline(std::cin, line)) {
        if (line == "Commands:") break;
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
            } else if (action == "jump") {
                int x, y;
                cmd >> x >> y;
                auto pos = pixelToCell(x, y, engine.getBoard().rows, engine.getBoard().cols);
                if (pos.has_value()) {
                    engine.requestJump(pos.value());
                }
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