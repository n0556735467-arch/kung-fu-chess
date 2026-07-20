#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../engine/game_engine.hpp"
#include "../../io/board_parser.hpp"

TEST_CASE("Enemy collision: capturing a moving-into enemy works correctly") {
    std::istringstream boardInput("wR . bP\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2000);

    const Piece* atDest = engine.getBoard().pieceAt(Position(0, 2));
    REQUIRE(atDest != nullptr);
    CHECK(atDest->color == Color::White);
}

TEST_CASE("Invalid premove: same piece cannot be redirected mid-motion") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.requestMove(Position(0, 0), Position(0, 1));
    engine.wait(2000);

    CHECK(engine.getBoard().pieceAt(Position(0, 2)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 1)) == nullptr);
}

TEST_CASE("Friendly-piece landing: move onto own-color piece is rejected") {
    std::istringstream boardInput("wR . wN\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));

    CHECK(engine.getLastMoveFrom().has_value() == false);
    CHECK(engine.getBoard().pieceAt(Position(0, 0)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 2)) != nullptr);
}

TEST_CASE("Two different pieces can move simultaneously") {
    std::istringstream boardInput("wR . .\n. . .\n. . bN\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 1));   // wR: תא אחד = 1000ms
    engine.requestMove(Position(2, 2), Position(1, 0));   // bN: קפיצת L חוקית! (-1,-2)

    engine.wait(1000);

    CHECK(engine.getBoard().pieceAt(Position(0, 0)) == nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 1)) != nullptr);

    CHECK(engine.getBoard().pieceAt(Position(2, 2)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(1, 0)) == nullptr);

    engine.wait(1000);

    CHECK(engine.getBoard().pieceAt(Position(2, 2)) == nullptr);
    CHECK(engine.getBoard().pieceAt(Position(1, 0)) != nullptr);
}