#include <sstream>
#include <string>
#include "script_runner.hpp"
#include "../io/board_printer.hpp"
#include "../input/board_mapper.hpp"

void ScriptRunner::run(std::istream& in, std::ostream& out, GameEngine& engine, Controller& controller) {
    std::string line;

    while (std::getline(in, line)) {
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
                printBoard(engine.getBoard(), out);
            }
        }
    }
}