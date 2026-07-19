// placeholder header for Motion
#ifndef MOTION_HPP
#define MOTION_HPP
#include "../model/position.hpp"

struct Motion {
    Position from;
    Position to;
    int remainingMs;

    Motion(Position from, Position to, int durationMs);
};

#endif