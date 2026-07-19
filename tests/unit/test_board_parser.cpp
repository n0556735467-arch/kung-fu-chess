// placeholder unit test for board parser (empty)
// tests/unit/test_board_parser.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../io/board_parser.hpp"

TEST_CASE("Parses a simple valid board") {
    std::istringstream input(
        "wK . bR\n"
        ". . .\n"
        ". wN bK\n"
    );

    Board b = parseBoard(input);

    CHECK(b.rows == 3);
    CHECK(b.cols == 3);
    CHECK(b.getPieces().size() == 4);
}

TEST_CASE("Parsed pieces have correct positions and kinds") {
    std::istringstream input(
        "wK . bR\n"
        ". . .\n"
        ". wN bK\n"
    );

    Board b = parseBoard(input);

    const Piece* wk = b.pieceAt(Position(0, 0));
    REQUIRE(wk != nullptr);
    CHECK(wk->color == Color::White);
    CHECK(wk->kind == Kind::King);

    const Piece* br = b.pieceAt(Position(0, 2));
    REQUIRE(br != nullptr);
    CHECK(br->color == Color::Black);
    CHECK(br->kind == Kind::Rook);
}

TEST_CASE("Throws on inconsistent row lengths") {
    std::istringstream input(
        "wK . bR\n"
        ". .\n"
        ". wN bK\n"
    );

    CHECK_THROWS_AS(parseBoard(input), InvalidBoardException);
}

TEST_CASE("Throws on unknown piece token") {
    std::istringstream input(
        "wK xZ\n"
        ". .\n"
    );

    CHECK_THROWS_AS(parseBoard(input), InvalidBoardException);
}