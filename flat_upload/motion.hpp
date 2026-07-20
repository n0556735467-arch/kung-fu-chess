// placeholder header for Motion
#ifndef MOTION_HPP
#define MOTION_HPP
#include "position.hpp"

struct Motion {
    int pieceId;
    Position from;
    Position to;
    int remainingMs;

    Motion(int pieceId, Position from, Position to, int durationMs);
};

#endif