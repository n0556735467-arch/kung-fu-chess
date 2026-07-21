#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../../io/board_printer.hpp"

TEST_CASE("Prints a board back in canonical form") {
    Board b(3, 3);
    b.addPiece(Piece(0, Color::White, Kind::King, Position(0, 0)));
    b.addPiece(Piece(1, Color::Black, Kind::Rook, Position(0, 2)));
    b.addPiece(Piece(2, Color::White, Kind::Knight, Position(2, 1)));
    b.addPiece(Piece(3, Color::Black, Kind::King, Position(2, 2)));

    std::ostringstream out;
    printBoard(b, out);

    std::string expected =
        "wK . bR\n"
        ". . .\n"
        ". wN bK\n";

    CHECK(out.str() == expected);
}

TEST_CASE("Prints an empty board correctly") {
    Board b(2, 2);

    std::ostringstream out;
    printBoard(b, out);

    std::string expected =
        ". .\n"
        ". .\n";

    CHECK(out.str() == expected);
}