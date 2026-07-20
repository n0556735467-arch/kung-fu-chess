// placeholder C++ file for Motion (empty)
#include "motion.hpp"

Motion::Motion(int pieceId, Position from, Position to, int durationMs)
    : pieceId(pieceId), from(from), to(to), remainingMs(durationMs) {}