#ifndef MOTION_SNAPSHOT_HPP
#define MOTION_SNAPSHOT_HPP
#include "../model/position.hpp"

struct MotionSnapshot {
    Position from;
    Position to;
    double progress;
};

#endif