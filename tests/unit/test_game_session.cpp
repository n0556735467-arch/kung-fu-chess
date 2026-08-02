#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/game_session.hpp"
#include "../../io/board_parser.hpp"
#include <sstream>

TEST_CASE("First connection is White and can move white pieces") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 4)));
    GameEngine engine(board);
    GameSession session(engine);

    session.handleCommand("conn1", "wP e2-e4");

    CHECK(engine.getMoveLog().size() == 1);
}

TEST_CASE("A White-role client cannot move Black pieces") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 4)));
    board.addPiece(Piece(1, Color::Black, Kind::Pawn, Position(1, 4)));
    GameEngine engine(board);
    GameSession session(engine);

    session.handleCommand("conn1", "bP e7-e5");

    CHECK(engine.getMoveLog().empty());
}

TEST_CASE("A third connection is an Observer and its commands are ignored") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 4)));
    GameEngine engine(board);
    GameSession session(engine);

    session.handleCommand("conn1", "");
    session.handleCommand("conn2", "");
    session.handleCommand("conn3", "wP e2-e4");

    CHECK(engine.getMoveLog().empty());
}

TEST_CASE("White client can request a jump for its own piece") {
    Board board(8, 8);
    board.addPiece(Piece(0, Color::White, Kind::Pawn, Position(6, 4)));
    GameEngine engine(board);
    GameSession session(engine);

    session.handleCommand("conn1", "JUMP wP e2");

    const Piece* piece = engine.pieceAt(Position(6, 4));
    REQUIRE(piece != nullptr);
    CHECK(piece->state == PieceState::Airborne);
}

TEST_CASE("LOGIN sets the player name for the connecting role") {
    Board board(8, 8);
    GameEngine engine(board);
    GameSession session(engine);

    session.handleCommand("conn1", "LOGIN Dana");
    session.handleCommand("conn2", "LOGIN Yosef");

    CHECK(session.getWhiteName() == "Dana");
    CHECK(session.getBlackName() == "Yosef");
}