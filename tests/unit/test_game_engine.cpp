#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../engine/game_engine.hpp"

TEST_CASE("Snapshot reflects board dimensions and piece data") {
    Board board(3, 3);
    board.addPiece(Piece(0, Color::White, Kind::King, Position(0, 0)));
    board.addPiece(Piece(1, Color::Black, Kind::Rook, Position(2, 2)));

    GameEngine engine(board);
    GameSnapshot snap = engine.snapshot();

    CHECK(snap.rows == 3);
    CHECK(snap.cols == 3);
    CHECK(snap.pieces.size() == 2);
    CHECK(snap.gameOver == false);
}

TEST_CASE("Snapshot gameOver becomes true after king capture") {
    Board board(1, 3);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));
    board.addPiece(Piece(1, Color::Black, Kind::King, Position(0, 2)));

    GameEngine engine(board);
    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2000);   // 2 cells * 1000ms/cell

    GameSnapshot snap = engine.snapshot();
    CHECK(snap.gameOver == true);
}