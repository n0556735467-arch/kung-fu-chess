#include <algorithm>
#include <cstdlib>
#include "real_time_arbiter.hpp"

bool RealTimeArbiter::hasActiveMotion() const {
    return activeMotion.has_value();
}

int RealTimeArbiter::distanceInCells(Position from, Position to) const {
    int rowDiff = std::abs(to.row - from.row);
    int colDiff = std::abs(to.col - from.col);
    return std::max(rowDiff, colDiff);
}

bool RealTimeArbiter::startMotion(Position from, Position to, Board& board) {
    if (hasActiveMotion()) {
        return false;
    }

    int cells = distanceInCells(from, to);
    int durationMs = cells * 1000;

    activeMotion = Motion(from, to, durationMs);
    return true;
}

void RealTimeArbiter::wait(int ms, Board& board) {
    if (!hasActiveMotion()) {
        return;
    }

    activeMotion->remainingMs -= ms;

    if (activeMotion->remainingMs <= 0) {
        const Piece* captured = board.pieceAt(activeMotion->to);
        if (captured != nullptr) {
            board.removePiece(activeMotion->to);
        }

        board.movePiece(activeMotion->from, activeMotion->to);
        activeMotion.reset();
    }
}