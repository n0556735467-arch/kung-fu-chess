#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../engine/game_engine.hpp"
#include "../../model/constants.hpp"

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

TEST_CASE("Snapshot includes motion progress for a moving piece") {
    Board board(1, 3);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));

    GameEngine engine(board);
    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(MOVE_DURATION_MS_PER_CELL);

    GameSnapshot snap = engine.snapshot();
    REQUIRE(snap.pieces.size() == 1);
    REQUIRE(snap.pieces[0].motion.has_value());
    CHECK(snap.pieces[0].motion->progress == doctest::Approx(0.5));
}

TEST_CASE("Capturing a pawn adds one point to the capturing side") {
    Board board(1, 3);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));
    board.addPiece(Piece(1, Color::Black, Kind::Pawn, Position(0, 2)));

    GameEngine engine(board);
    engine.requestMove(Position(0, 0), Position(0, 2));
    engine.wait(2 * MOVE_DURATION_MS_PER_CELL);

    CHECK(engine.whiteScore() == 1);
    CHECK(engine.blackScore() == 0);
}

TEST_CASE("Move log records each move with elapsed time and square names") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 4)));

    GameEngine engine(board);
    engine.wait(500);
    engine.requestMove(Position(6, 4), Position(4, 4));

    const std::vector<MoveLogEntry>& log = engine.getMoveLog();
    REQUIRE(log.size() == 1);
    CHECK(log[0].timeMs == 500);
    CHECK(log[0].text == "wP e2-e4");
}