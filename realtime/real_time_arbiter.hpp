#ifndef REAL_TIME_ARBITER_HPP
#define REAL_TIME_ARBITER_HPP
#include <vector>
#include "motion.hpp"
#include "jump.hpp"
#include "../model/board.hpp"
#include <optional>
#include "motion_snapshot.hpp"
#include "captured_piece.hpp"

class RealTimeArbiter {
public:
    bool startMotion(Position from, Position to, Board& board);
    bool startJump(Position at, Board& board);
    bool wait(int ms, Board& board);
    std::optional<MotionSnapshot> activeMotionFor(int pieceId) const;
    std::vector<CapturedPiece> consumeCaptures();

private:
    std::vector<Motion> activeMotions;
    std::vector<Jump> activeJumps;
    int distanceInCells(Position from, Position to) const;
    bool hasMotionFrom(Position from) const;
    std::vector<CapturedPiece> pendingCaptures;

};

#endif