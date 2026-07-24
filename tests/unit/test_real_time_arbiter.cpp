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

TEST_CASE("A piece cannot be redirected while already moving") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 2));  // 2 תאים = 2000ms
    engine.requestMove(Position(0, 0), Position(0, 1));  // נסיון הסטה - אמור להידחות

    engine.wait(2000);

    CHECK(engine.getBoard().pieceAt(Position(0, 2)) != nullptr);
    CHECK(engine.getBoard().pieceAt(Position(0, 1)) == nullptr);
}

TEST_CASE("After arrival, a piece can move again immediately with no cooldown") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 1));
    engine.wait(1000);

    engine.requestMove(Position(0, 1), Position(0, 2));
    engine.wait(1000);

    CHECK(engine.getBoard().pieceAt(Position(0, 2)) != nullptr);
}

TEST_CASE("Pawn promotes to Queen upon reaching the last row") {
    std::istringstream boardInput(". . .\nwP . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(1, 0), Position(0, 0));
    engine.wait(1000);

    const Piece* promoted = engine.getBoard().pieceAt(Position(0, 0));
    REQUIRE(promoted != nullptr);
    CHECK(promoted->kind == Kind::Queen);
}

TEST_CASE("Piece state changes to Moving during motion and back to Idle on arrival") {
    std::istringstream boardInput("wR . .\n. . .\n. . .\n");
    Board board = parseBoard(boardInput);
    GameEngine engine(board);

    engine.requestMove(Position(0, 0), Position(0, 1));

    const Piece* duringMove = engine.getBoard().pieceAt(Position(0, 0));
    REQUIRE(duringMove != nullptr);
    CHECK(duringMove->state == PieceState::Moving);

    engine.wait(1000);

    const Piece* afterArrival = engine.getBoard().pieceAt(Position(0, 1));
    REQUIRE(afterArrival != nullptr);
    CHECK(afterArrival->state == PieceState::Idle);
}