#include <algorithm>
#include <cstdlib>
#include <iostream>

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

bool RealTimeArbiter::wait(int ms, Board& board) {
    for (Motion& m : activeMotions) {
        m.remainingMs -= ms;
    }

    for (size_t i = 0; i < activeMotions.size();) {
        if (activeMotions[i].remainingMs <= 0) {
            int pieceId = activeMotions[i].pieceId;
            Position to = activeMotions[i].to;

            Piece* moving = board.findPieceById(pieceId);

            if (moving != nullptr) {
                const Piece* captured = board.pieceAt(to);
                if (captured != nullptr && captured->id != pieceId) {
                    board.removePiece(to);
                }

                moving = board.findPieceById(pieceId);  // מצביע טרי, אחרי השינוי בוקטור
                if (moving != nullptr) {
                    moving->cell = to;
                }
            }

            activeMotions.erase(activeMotions.begin() + i);
        } else {
            i++;
        }
    }
}