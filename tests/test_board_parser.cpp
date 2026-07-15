#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../src/BoardParser.h"

TEST_CASE("Parses a simple valid board") {
    std::istringstream input(
        "wK . bR\n"
        ". . .\n"
        ". wN bK\n"
    );

    Board b = parseBoard(input);

    CHECK(b.rows == 3);
    CHECK(b.cols == 3);
    CHECK(b.pieces.size() == 4);
}

TEST_CASE("Throws on inconsistent row lengths") {
    std::istringstream input(
        "wK . bR\n"
        ". .\n"
        ". wN bK\n"
    );

    CHECK_THROWS_AS(parseBoard(input), InvalidBoardException);
}