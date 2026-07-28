#include <algorithm>
#include <cstdlib>
#include "real_time_arbiter.hpp"
#include "../model/constants.hpp"

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
    int durationMs = cells * MOVE_DURATION_MS_PER_CELL;

    activeMotions.push_back(Motion(piece->id, from, to, durationMs));
    
    Piece* mutablePiece = board.findPieceById(piece->id);
    if (mutablePiece != nullptr) {
        mutablePiece->state = PieceState::Moving;
    }
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

    activeJumps.push_back(Jump(piece->id, JUMP_DURATION_MS));
    
    Piece* mutablePiece = board.findPieceById(piece->id);
    if (mutablePiece != nullptr) {
        mutablePiece->state = PieceState::Airborne;
    }
    
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
                    bool friendlyBlock = false;

                    if (captured != nullptr && captured->id != pieceId) {
                        if (captured->color == moving->color) {
                            friendlyBlock = true;
                        } else {
                            if (captured->kind == Kind::King) {
                                kingCaptured = true;
                            }
                            board.removePiece(to);
                        }
                    }

                    if (friendlyBlock) {
                        moving->state = PieceState::Idle;
                    } else {
                        moving = board.findPieceById(pieceId);
                        if (moving != nullptr) {
                            moving->cell = to;
                            moving->state = PieceState::Idle;

                            if (moving->kind == Kind::Pawn) {
                                int lastRow = (moving->color == Color::White) ? 0 : board.rows - 1;
                                if (moving->cell.row == lastRow) {
                                    moving->kind = Kind::Queen;
                                }
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
            Piece* p = board.findPieceById(activeJumps[i].pieceId);
            if (p != nullptr) {
                p->state = PieceState::Idle;
            }
            activeJumps.erase(activeJumps.begin() + i);
        } else {
            i++;
        }
    }

    return kingCaptured;
}

std::optional<MotionSnapshot> RealTimeArbiter::activeMotionFor(int pieceId) const {
    for (const Motion& m : activeMotions) {
        if (m.pieceId == pieceId) {
            double progress = 1.0 - static_cast<double>(m.remainingMs) / static_cast<double>(m.totalMs);
            if (progress < 0.0) progress = 0.0;
            if (progress > 1.0) progress = 1.0;
            return MotionSnapshot{m.from, m.to, progress};
        }
    }
    return std::nullopt;
}