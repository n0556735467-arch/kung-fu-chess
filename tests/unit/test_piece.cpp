#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../model/piece.hpp"

TEST_CASE("Piece stores id, color, kind and position correctly") {
    Piece p(0, Color::White, Kind::Queen, Position(0, 4));
    CHECK(p.id == 0);
    CHECK(p.color == Color::White);
    CHECK(p.kind == Kind::Queen);
    CHECK(p.cell == Position(0, 4));
}

TEST_CASE("Two pieces can have different ids with same color and kind") {
    Piece p1(0, Color::White, Kind::Pawn, Position(1, 0));
    Piece p2(1, Color::White, Kind::Pawn, Position(1, 1));
    CHECK(p1.id != p2.id);
}