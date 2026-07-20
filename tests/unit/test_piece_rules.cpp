// tests/unit/test_piece_rules.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../rules/piece_rules.hpp"
#include "../../model/board.hpp"

TEST_CASE("Rook moves across empty row and column") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));

    RookRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(0, 0)));

    bool foundRow = false, foundCol = false;
    for (const Position& p : dests) {
        if (p == Position(0, 7)) foundRow = true;
        if (p == Position(7, 0)) foundCol = true;
    }
    CHECK(foundRow);
    CHECK(foundCol);
}

TEST_CASE("Rook stops before a friendly blocker") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));
    board.addPiece(Piece(1, Color::White, Kind::Pawn, Position(0, 3)));

    RookRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(0, 0)));

    bool foundBlocker = false, foundBeyond = false;
    for (const Position& p : dests) {
        if (p == Position(0, 3)) foundBlocker = true;
        if (p == Position(0, 4)) foundBeyond = true;
    }
    CHECK_FALSE(foundBlocker);
    CHECK_FALSE(foundBeyond);
}

TEST_CASE("Rook captures an enemy blocker but does not pass it") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Rook, Position(0, 0)));
    board.addPiece(Piece(1, Color::Black, Kind::Pawn, Position(0, 3)));

    RookRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(0, 0)));

    bool foundCapture = false, foundBeyond = false;
    for (const Position& p : dests) {
        if (p == Position(0, 3)) foundCapture = true;
        if (p == Position(0, 4)) foundBeyond = true;
    }
    CHECK(foundCapture);
    CHECK_FALSE(foundBeyond);
}

TEST_CASE("Bishop moves diagonally and not straight") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Bishop, Position(4, 4)));

    BishopRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(4, 4)));

    bool foundDiagonal = false, foundStraight = false;
    for (const Position& p : dests) {
        if (p == Position(2, 2)) foundDiagonal = true;
        if (p == Position(4, 0)) foundStraight = true;
    }
    CHECK(foundDiagonal);
    CHECK_FALSE(foundStraight);
}

TEST_CASE("Queen combines rook and bishop movement") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Queen, Position(4, 4)));

    QueenRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(4, 4)));

    bool foundStraight = false, foundDiagonal = false;
    for (const Position& p : dests) {
        if (p == Position(4, 0)) foundStraight = true;
        if (p == Position(2, 2)) foundDiagonal = true;
    }
    CHECK(foundStraight);
    CHECK(foundDiagonal);
}

TEST_CASE("Knight jumps over blockers") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Knight, Position(4, 4)));
    board.addPiece(Piece(1, Color::White, Kind::Pawn, Position(4, 5)));
    board.addPiece(Piece(2, Color::White, Kind::Pawn, Position(5, 4)));

    KnightRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(4, 4)));

    bool foundJump = false;
    for (const Position& p : dests) {
        if (p == Position(2, 3)) foundJump = true;
    }
    CHECK(foundJump);
}

TEST_CASE("King moves one cell only") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::King, Position(4, 4)));

    KingRule rule;
    std::vector<Position> dests = rule.legalDestinations(board, *board.pieceAt(Position(4, 4)));

    bool foundOneCell = false, foundTwoCells = false;
    for (const Position& p : dests) {
        if (p == Position(4, 5)) foundOneCell = true;
        if (p == Position(4, 6)) foundTwoCells = true;
    }
    CHECK(foundOneCell);
    CHECK_FALSE(foundTwoCells);
}

TEST_CASE("White pawn moves one row upward to empty cell") {
    Board board(4, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(1, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(1, 1)));

    bool foundForward = false, foundDouble = false;
    for (auto& p : dests) {
        if (p == Position(0, 1)) foundForward = true;
        if (p == Position(-1, 1)) foundDouble = true;
    }
    CHECK(foundForward == true);
    CHECK(foundDouble == false);
}

TEST_CASE("White pawn can move two cells from its start row when path is clear") {
    Board board(8, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(7, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(7, 1)));

    bool foundDouble = false;
    for (auto& p : dests) {
        if (p == Position(5, 1)) foundDouble = true;
    }
    CHECK(foundDouble == true);
}

TEST_CASE("Pawn cannot move two cells if not on its start row") {
    Board board(8, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(5, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(5, 1)));

    bool foundDouble = false;
    for (auto& p : dests) {
        if (p == Position(3, 1)) foundDouble = true;
    }
    CHECK(foundDouble == false);
}

TEST_CASE("Pawn cannot jump over a blocker for double move") {
    Board board(8, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(7, 1)));
    board.addPiece(Piece(1, Color::Black, Kind::Pawn, Position(6, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(7, 1)));

    bool foundOne = false, foundTwo = false;
    for (auto& p : dests) {
        if (p == Position(6, 1)) foundOne = true;
        if (p == Position(5, 1)) foundTwo = true;
    }
    CHECK(foundOne == false);
    CHECK(foundTwo == false);
}

TEST_CASE("Black pawn moves one row downward to empty cell") {
    Board board(4, 3);
    board.addPiece(Piece(0, Color::Black, Kind::Pawn, Position(1, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(1, 1)));

    bool foundForward = false;
    for (auto& p : dests) {
        if (p == Position(2, 1)) foundForward = true;
    }
    CHECK(foundForward == true);
}

TEST_CASE("Pawn captures diagonally but not straight forward") {
    Board board(4, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(2, 1)));
    board.addPiece(Piece(1, Color::Black, Kind::Pawn, Position(1, 0)));
    board.addPiece(Piece(2, Color::Black, Kind::Pawn, Position(1, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(2, 1)));

    bool foundDiagonalCapture = false, foundStraightCapture = false;
    for (auto& p : dests) {
        if (p == Position(1, 0)) foundDiagonalCapture = true;
        if (p == Position(1, 1)) foundStraightCapture = true;
    }
    CHECK(foundDiagonalCapture == true);
    CHECK(foundStraightCapture == false);
}

TEST_CASE("White pawn can move two cells from its start row when path is clear") {
    Board board(8, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(6, 1)));

    bool foundDouble = false;
    for (auto& p : dests) {
        if (p == Position(4, 1)) foundDouble = true;
    }
    CHECK(foundDouble == true);
}

TEST_CASE("Pawn cannot move two cells if not on its start row") {
    Board board(8, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(5, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(5, 1)));

    bool foundDouble = false;
    for (auto& p : dests) {
        if (p == Position(3, 1)) foundDouble = true;
    }
    CHECK(foundDouble == false);
}

TEST_CASE("Pawn cannot jump over a blocker for double move") {
    Board board(8, 3);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 1)));
    board.addPiece(Piece(1, Color::Black, Kind::Pawn, Position(5, 1)));

    PawnRule rule;
    auto dests = rule.legalDestinations(board, *board.pieceAt(Position(6, 1)));

    bool foundOne = false, foundTwo = false;
    for (auto& p : dests) {
        if (p == Position(5, 1)) foundOne = true;
        if (p == Position(4, 1)) foundTwo = true;
    }
    CHECK(foundOne == false);
    CHECK(foundTwo == false);
}