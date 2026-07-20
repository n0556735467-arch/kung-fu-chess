// placeholder unit test for real time arbiter (empty)
// tests/unit/test_real_time_arbiter.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../engine/game_engine.hpp"
#include "../../io/board_parser.hpp"

TEST_CASE("Board does not change before motion completes") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(1500);

    CHECK(engine.getBoard().pieceAt(Position(0, 0)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 2)) == nullptr);
}

TEST_CASE("Board updates after motion completes") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2000);

    CHECK(engine.getBoard().pieceAt(Position(0, 0)) == nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 2)) != nullptr);
}

TEST_CASE("Capturing an enemy piece removes it from the board on arrival") {
    std::istringstream boardInput("wR . bP\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2000);

    const Piece* atDest = engine.getBoard().pieceAt(Position(0, 2));
    REQUIRE(atDest != nullptr);
    CHECK(atDest->color == Color::White);
    CHECK(atDest->kind == Kind::Rook);
}

TEST_CASE("Multiple partial waits accumulate until motion completes") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 1));

    engine.wait(500);
    CHECK(engine.getBoard().pieceAt(Position(0, 0)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 1)) == nullptr);

    engine.wait(600);
    CHECK(engine.getBoard().pieceAt(Position(0, 0)) == nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 1)) != nullptr);
}