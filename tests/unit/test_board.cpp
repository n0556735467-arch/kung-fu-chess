#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../model/board.hpp"

TEST_CASE("Board dimensions are correct") {
    Board b(8, 8);
    CHECK(b.rows == 8);
    CHECK(b.cols == 8);
}

TEST_CASE("Empty cell returns no piece") {
    Board b(8, 8);
    CHECK(b.pieceAt(Position(3, 3)) == nullptr);
}

TEST_CASE("Occupied cell returns the correct piece") {
    Board b(8, 8);
    b.addPiece(Piece(0, Color::White, Kind::King, Position(0, 4)));
    const Piece* p = b.pieceAt(Position(0, 4));
    REQUIRE(p != nullptr);
    CHECK(p->kind == Kind::King);
}

TEST_CASE("Adding two pieces to the same cell fails") {
    Board b(8, 8);
    b.addPiece(Piece(0, Color::White, Kind::King, Position(0, 4)));
    CHECK_THROWS_AS(
        b.addPiece(Piece(1, Color::Black, Kind::Queen, Position(0, 4))),
        std::runtime_error
    );
}

TEST_CASE("Moving a piece updates source and destination") {
    Board b(8, 8);
    b.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));
    b.movePiece(Position(0, 0), Position(0, 5));
    CHECK(b.pieceAt(Position(0, 0)) == nullptr);
    CHECK(b.pieceAt(Position(0, 5)) != nullptr);
}

TEST_CASE("Removing a piece clears its cell") {
    Board b(8, 8);
    b.addPiece(Piece(0, Color::White, Kind::Pawn, Position(1, 0)));
    b.removePiece(Position(1, 0));
    CHECK(b.pieceAt(Position(1, 0)) == nullptr);
}