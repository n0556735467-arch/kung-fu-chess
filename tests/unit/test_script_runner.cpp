#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../texttests/script_runner.hpp"
#include "../../io/board_parser.hpp"

TEST_CASE("ScriptRunner dispatches click and print board correctly") {
    std::istringstream boardInput("wK . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);
    Controller controller(engine);

    std::istringstream commands("print board\n");
    std::ostringstream output;

    ScriptRunner runner;
    runner.run(commands, output, engine, controller);

    std::string expected = "wK . .\n. . .\n. . .\n";
    CHECK(output.str() == expected);
}