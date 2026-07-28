#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../rules/rule_engine.hpp"

TEST_CASE("King moving two cells is rejected") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::King, Position(4, 4)));

    RuleEngine engine;
    MoveValidation result = engine.validateMove(board, Position(4, 4), Position(4, 6));

    CHECK_FALSE(result.isValid);
    CHECK(result.reason == "illegal_piece_move");
}

TEST_CASE("Rook moving diagonally is rejected") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));

    RuleEngine engine;
    MoveValidation result = engine.validateMove(board, Position(0, 0), Position(3, 3));

    CHECK_FALSE(result.isValid);
    CHECK(result.reason == "illegal_piece_move");
}

TEST_CASE("Valid rook move is accepted") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));

    RuleEngine engine;
    MoveValidation result = engine.validateMove(board, Position(0, 0), Position(0, 5));

    CHECK(result.isValid);
    CHECK(result.reason == "ok");
}

TEST_CASE("legalDestinations forwards to the piece's own rule") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));

    RuleEngine engine;
    std::vector<Position> destinations = engine.legalDestinations(board, Position(0, 0));

    bool foundHorizontal = false;
    bool foundVertical = false;
    for (const Position& p : destinations) {
        if (p == Position(0, 5)) foundHorizontal = true;
        if (p == Position(5, 0)) foundVertical = true;
    }

    CHECK(foundHorizontal);
    CHECK(foundVertical);
}

TEST_CASE("legalDestinations returns empty for an empty cell") {
    Board board(8, 8);
    RuleEngine engine;
    std::vector<Position> destinations = engine.legalDestinations(board, Position(3, 3));
    CHECK(destinations.empty());
}