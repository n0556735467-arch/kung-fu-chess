#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../engine/game_engine.hpp"
#include "../../io/board_parser.hpp"

TEST_CASE("A piece cannot jump if it does not exist at that cell") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    CHECK(engine.requestJump(Position(1, 1)) == false);
}

TEST_CASE("A moving piece cannot jump") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    CHECK(engine.requestJump(Position(0, 0)) == false);
}

TEST_CASE("Airborne piece captures an enemy that arrives during the jump window") {
    std::istringstream boardInput("wR bR .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    REQUIRE(engine.requestJump(Position(0, 0)));
    engine.requestMove(Position(0, 1), Position(0, 0));
    engine.wait(1000);

    const Piece* atOrigin = engine.getBoard().pieceAt(Position(0, 0));
    REQUIRE(atOrigin != nullptr);
    CHECK(atOrigin->color == Color::White);
    CHECK(engine.getBoard().pieceAt(Position(0, 1)) == nullptr);
}

TEST_CASE("No air capture happens if no enemy arrives during the jump") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    REQUIRE(engine.requestJump(Position(0, 0)));
    engine.wait(1000);

    CHECK(engine.getBoard().pieceAt(Position(0, 0)) != nullptr);
}

TEST_CASE("After a jump ends, the piece can jump again") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestJump(Position(0, 0));
    engine.wait(1000);

    CHECK(engine.requestJump(Position(0, 0)) == true);
}