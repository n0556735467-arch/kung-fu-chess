#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../input/controller.hpp"
#include "../../io/board_parser.hpp"

TEST_CASE("First click on a piece sets selected cell") {
    std::istringstream boardInput("wK . .\n. . .\n. . bK\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);
    Controller controller(engine);

    controller.click(50, 50);

    CHECK(controller.hasSelection() == true);
    CHECK(controller.selectedPosition() == Position(0, 0));
}

TEST_CASE("First click on an empty cell leaves selection empty") {
    std::istringstream boardInput("wK . .\n. . .\n. . bK\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);
    Controller controller(engine);

    controller.click(150, 50);

    CHECK(controller.hasSelection() == false);
}

TEST_CASE("Outside click with no selection does nothing") {
    std::istringstream boardInput("wK . .\n. . .\n. . bK\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);
    Controller controller(engine);

    controller.click(-10, 50);

    CHECK(controller.hasSelection() == false);
}

TEST_CASE("Outside click with selected piece clears selection and does not call GameEngine") {
    std::istringstream boardInput("wK . .\n. . .\n. . bK\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);
    Controller controller(engine);

    controller.click(50, 50);
    CHECK(controller.hasSelection() == true);

    controller.click(-10, 50);

    CHECK(controller.hasSelection() == false);
    CHECK(engine.getLastMoveFrom().has_value() == false);
}

TEST_CASE("Second in-board click sends the correct source and destination and clears selection") {
    std::istringstream boardInput("wK . .\n. . .\n. . bK\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);
    Controller controller(engine);

    controller.click(50, 50);
    controller.click(150, 150);

    REQUIRE(engine.getLastMoveFrom().has_value());
    REQUIRE(engine.getLastMoveTo().has_value());
    CHECK(engine.getLastMoveFrom().value() == Position(0, 0));
    CHECK(engine.getLastMoveTo().value() == Position(1, 1));
    CHECK(controller.hasSelection() == false);
}