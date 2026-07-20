#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../engine/game_engine.hpp"
#include "../../io/board_parser.hpp"

TEST_CASE("Capturing the enemy king ends the game") {
    std::istringstream boardInput("wR . bK .\n. . . .\n. . . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2000);

    CHECK(engine.isGameOver() == true);

    const Piece* atDest = engine.getBoard().pieceAt(Position(0, 2));
    REQUIRE(atDest != nullptr);
    CHECK(atDest->kind == Kind::Rook);
}

TEST_CASE("After game over, further move requests are ignored") {
    std::istringstream boardInput("wR . bK .\n. wN . .\n. . . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2000);
    REQUIRE(engine.isGameOver());

    engine.requestMove(Position(1, 1), Position(2, 3));
    engine.wait(5000);

    CHECK(engine.getBoard().pieceAt(Position(1, 1)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(2, 3)) == nullptr);
}