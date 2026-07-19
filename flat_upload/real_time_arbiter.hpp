// placeholder header for real time arbiter
#ifndef REAL_TIME_ARBITER_HPP
#define REAL_TIME_ARBITER_HPP
#include <optional>
#include "motion.hpp"
#include "board.hpp"

class RealTimeArbiter {
public:
    bool hasActiveMotion() const;
    bool startMotion(Position from, Position to, Board& board);
    void wait(int ms, Board& board);

private:
    std::optional<Motion> activeMotion;
    int distanceInCells(Position from, Position to) const;
};

#endif