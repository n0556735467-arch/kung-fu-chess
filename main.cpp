#include <iostream>
#include <sstream>
#include <string>
#include "io/board_parser.hpp"
#include "io/board_printer.hpp"
#include "io/script_runner.hpp"
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

        ScriptRunner runner;
        runner.run(std::cin, std::cout, engine, controller);
        
    } catch (const InvalidBoardException& e) {
        if (e.type == BoardErrorType::UnknownToken) {
            std::cout << "ERROR UNKNOWN_TOKEN" << std::endl;
        } else {
            std::cout << "ERROR ROW_WIDTH_MISMATCH" << std::endl;
        }
    }

    return 0;
}