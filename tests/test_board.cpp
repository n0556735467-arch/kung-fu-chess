#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Board.h"

TEST_CASE("Board stores rows and cols correctly") {
    std::vector<Piece> pieces;
    Board b(8, 8, pieces);
    CHECK(b.rows == 8);
    CHECK(b.cols == 8);
    CHECK(b.pieces.size() == 0);
}

TEST_CASE("Board stores a vector of pieces correctly") {
    std::vector<Piece> pieces;
    pieces.push_back(Piece(Color::White, Kind::King, Position(0, 4)));
    pieces.push_back(Piece(Color::Black, Kind::King, Position(7, 4)));

    Board b(8, 8, pieces);

    CHECK(b.pieces.size() == 2);
    CHECK(b.pieces[0].color == Color::White);
    CHECK(b.pieces[0].kind == Kind::King);
    CHECK(b.pieces[1].color == Color::Black);
}