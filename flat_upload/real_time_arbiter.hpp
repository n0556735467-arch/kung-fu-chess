#ifndef REAL_TIME_ARBITER_HPP
#define REAL_TIME_ARBITER_HPP
#include <vector>
#include "motion.hpp"
#include "jump.hpp"
#include "board.hpp"

class RealTimeArbiter {
public:
    bool startMotion(Position from, Position to, Board& board);
    bool startJump(Position at, Board& board);
    bool wait(int ms, Board& board);

private:
    std::vector<Motion> activeMotions;
    std::vector<Jump> activeJumps;
    int distanceInCells(Position from, Position to) const;
    bool hasMotionFrom(Position from) const;
};

#endif