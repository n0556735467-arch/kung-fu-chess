#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include "../src/BoardPrinter.h"

TEST_CASE("Prints a board back in canonical form") {
    std::vector<Piece> pieces;
    pieces.push_back(Piece(Color::White, Kind::King, Position(0, 0)));
    pieces.push_back(Piece(Color::Black, Kind::Rook, Position(0, 2)));
    pieces.push_back(Piece(Color::White, Kind::kNight, Position(2, 1)));
    pieces.push_back(Piece(Color::Black, Kind::King, Position(2, 2)));

    Board b(3, 3, pieces);

    std::ostringstream out;
    printBoard(b, out);

    std::string expected =
        "wK . bR\n"
        ". . .\n"
        ". wN bK\n";

    CHECK(out.str() == expected);
}