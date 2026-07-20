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

bool RealTimeArbiter::startJump(Position at, Board& board) {
    const Piece* piece = board.pieceAt(at);
    if (piece == nullptr) {
        return false;
    }

    for (const Motion& m : activeMotions) {
        if (m.pieceId == piece->id) {
            return false;
        }
    }

    for (const Jump& j : activeJumps) {
        if (j.pieceId == piece->id) {
            return false;
        }
    }

    activeJumps.push_back(Jump(piece->id, 1000));
    return true;
}

bool RealTimeArbiter::wait(int ms, Board& board) {
    bool kingCaptured = false;

    for (Motion& m : activeMotions) {
        m.remainingMs -= ms;
    }
    for (Jump& j : activeJumps) {
        j.remainingMs -= ms;
    }

    for (size_t i = 0; i < activeMotions.size();) {
        if (activeMotions[i].remainingMs <= 0) {
            int pieceId = activeMotions[i].pieceId;
            Position from = activeMotions[i].from;
            Position to = activeMotions[i].to;

            const Piece* defender = board.pieceAt(to);
            bool airCapture = false;

            if (defender != nullptr) {
                const Piece* arriving = board.findPieceById(pieceId);
                if (arriving != nullptr && defender->color != arriving->color) {
                    for (const Jump& j : activeJumps) {
                        if (j.pieceId == defender->id) {
                            airCapture = true;
                            break;
                        }
                    }
                }
            }

            if (airCapture) {
                board.removePiece(from);
            } else {
                Piece* moving = board.findPieceById(pieceId);
                if (moving != nullptr) {
                    const Piece* captured = board.pieceAt(to);
                    if (captured != nullptr && captured->id != pieceId) {
                        if (captured->kind == Kind::King) {
                            kingCaptured = true;
                        }
                        board.removePiece(to);
                    }

                    moving = board.findPieceById(pieceId);
                    if (moving != nullptr) {
                        moving->cell = to;

                        if (moving->kind == Kind::Pawn) {
                            int lastRow = (moving->color == Color::White) ? 0 : board.rows - 1;
                            if (moving->cell.row == lastRow) {
                                moving->kind = Kind::Queen;
                            }
                        }
                    }
                }
            }

            activeMotions.erase(activeMotions.begin() + i);
        } else {
            i++;
        }
    }

    for (size_t i = 0; i < activeJumps.size();) {
        if (activeJumps[i].remainingMs <= 0) {
            activeJumps.erase(activeJumps.begin() + i);
        } else {
            i++;
        }
    }

    return kingCaptured;
}