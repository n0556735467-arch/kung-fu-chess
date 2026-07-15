#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Piece.h"


TEST_CASE("Piece stores color, kind and position correctly") {
    Piece p(Color::White, Kind::Queen, Position(0, 4));   
    CHECK(p.color == Color::White);
    CHECK(p.kind == Kind::Queen);
    CHECK(p.cell == Position(0, 4));
}