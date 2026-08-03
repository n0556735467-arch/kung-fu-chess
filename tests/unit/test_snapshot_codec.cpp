#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/snapshot_codec.hpp"

TEST_CASE("Round-trips a snapshot with a static piece") {
    GameSnapshot snap;
    snap.rows = 8;
    snap.cols = 8;
    snap.gameOver = false;
    snap.whiteScore = 3;
    snap.blackScore = 1;
    snap.pieces.push_back(PieceSnapshot{0, Color::White, Kind::Pawn, PieceState::Idle, Position(6, 4), std::nullopt});
    snap.moveLog.push_back(MoveLogEntry{500, "wP e2-e4"});

    std::string text = encodeSnapshot(snap);
    GameSnapshot decoded = decodeSnapshot(text);

    CHECK(decoded.rows == 8);
    CHECK(decoded.cols == 8);
    CHECK(decoded.gameOver == false);
    CHECK(decoded.whiteScore == 3);
    CHECK(decoded.blackScore == 1);
    REQUIRE(decoded.pieces.size() == 1);
    CHECK(decoded.pieces[0].id == 0);
    CHECK(decoded.pieces[0].color == Color::White);
    CHECK(decoded.pieces[0].kind == Kind::Pawn);
    CHECK(decoded.pieces[0].state == PieceState::Idle);
    CHECK(decoded.pieces[0].cell == Position(6, 4));
    CHECK_FALSE(decoded.pieces[0].motion.has_value());
    REQUIRE(decoded.moveLog.size() == 1);
    CHECK(decoded.moveLog[0].timeMs == 500);
    CHECK(decoded.moveLog[0].text == "wP e2-e4");
}

TEST_CASE("Round-trips a piece with active motion") {
    GameSnapshot snap;
    snap.rows = 8;
    snap.cols = 8;
    snap.gameOver = true;
    snap.whiteScore = 0;
    snap.blackScore = 0;
    snap.pieces.push_back(PieceSnapshot{
        1, Color::Black, Kind::Rook, PieceState::Moving, Position(0, 0),
        MotionSnapshot{Position(0, 0), Position(0, 2), 0.5}
    });

    std::string text = encodeSnapshot(snap);
    GameSnapshot decoded = decodeSnapshot(text);

    REQUIRE(decoded.pieces.size() == 1);
    REQUIRE(decoded.pieces[0].motion.has_value());
    CHECK(decoded.pieces[0].motion->from == Position(0, 0));
    CHECK(decoded.pieces[0].motion->to == Position(0, 2));
    CHECK(decoded.pieces[0].motion->progress == doctest::Approx(0.5));
    CHECK(decoded.gameOver == true);
}

TEST_CASE("Round-trips a snapshot with a winner") {
    GameSnapshot snap;
    snap.rows = 8;
    snap.cols = 8;
    snap.gameOver = true;
    snap.whiteScore = 0;
    snap.blackScore = 0;
    snap.winner = Color::Black;

    std::string text = encodeSnapshot(snap);
    GameSnapshot decoded = decodeSnapshot(text);

    REQUIRE(decoded.winner.has_value());
    CHECK(decoded.winner.value() == Color::Black);
}

TEST_CASE("Round-trips a snapshot with no winner yet") {
    GameSnapshot snap;
    snap.rows = 8;
    snap.cols = 8;
    snap.gameOver = false;
    snap.whiteScore = 0;
    snap.blackScore = 0;

    std::string text = encodeSnapshot(snap);
    GameSnapshot decoded = decodeSnapshot(text);

    CHECK_FALSE(decoded.winner.has_value());
}