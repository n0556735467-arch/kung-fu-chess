#include <algorithm>
#include <cstdlib>
#include "real_time_arbiter.hpp"

int RealTimeArbiter::distanceInCells(Position from, Position to) const {
    int rowDiff = std::abs(to.row - from.row);
    int colDiff = std::abs(to.col - from.col);
    return std::max(rowDiff, colDiff);
}

bool RealTimeArbiter::hasMotionFrom(Position from) const {
    for (const Motion& m : activeMotions) {
        if (m.from == from) {
            return true;
        }
    }
    return false;
}

bool RealTimeArbiter::startMotion(Position from, Position to, Board& board) {
    if (hasMotionFrom(from)) {
        return false;
    }

    const Piece* piece = board.pieceAt(from);
    if (piece == nullptr) {
        return false;
    }

    int cells = distanceInCells(from, to);
    int durationMs = cells * 1000;

    activeMotions.push_back(Motion(piece->id, from, to, durationMs));
    return true;
}

void RealTimeArbiter::wait(int ms, Board& board) {
    for (Motion& m : activeMotions) {
        m.remainingMs -= ms;
    }

    for (size_t i = 0; i < activeMotions.size();) {
        if (activeMotions[i].remainingMs <= 0) {
            Piece* moving = board.findPieceById(activeMotions[i].pieceId);

            if (moving != nullptr) {
                const Piece* captured = board.pieceAt(activeMotions[i].to);
                if (captured != nullptr && captured->id != moving->id) {
                    board.removePiece(activeMotions[i].to);
                }
                moving->cell = activeMotions[i].to;
            }

            activeMotions.erase(activeMotions.begin() + i);
        } else {
            i++;
        }
    }
}