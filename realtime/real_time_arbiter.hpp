#ifndef REAL_TIME_ARBITER_HPP
#define REAL_TIME_ARBITER_HPP
#include <vector>
#include "motion.hpp"
#include "../model/board.hpp"

class RealTimeArbiter {
public:
    bool startMotion(Position from, Position to, Board& board);
    void wait(int ms, Board& board);

private:
    std::vector<Motion> activeMotions;
    int distanceInCells(Position from, Position to) const;
    bool hasMotionFrom(Position from) const;
};

#endif