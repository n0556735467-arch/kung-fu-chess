#include "rule_engine.hpp"
#include "piece_rules.hpp"

MoveValidation RuleEngine::validateMove(const Board& board, Position from, Position to) const {
    if (!board.isInBounds(from) || !board.isInBounds(to)) {
        return MoveValidation{false, "outside_board"};
    }

    const Piece* source = board.pieceAt(from);
    if (source == nullptr) {
        return MoveValidation{false, "empty_source"};
    }

    const Piece* destPiece = board.pieceAt(to);
    if (destPiece != nullptr && destPiece->color == source->color) {
        return MoveValidation{false, "friendly_destination"};
    }

    std::vector<Position> legal;
    switch (source->kind) {
        case Kind::Rook:   { RookRule r;   legal = r.legalDestinations(board, *source); break; }
        case Kind::Bishop: { BishopRule r; legal = r.legalDestinations(board, *source); break; }
        case Kind::Queen:  { QueenRule r;  legal = r.legalDestinations(board, *source); break; }
        case Kind::Knight: { KnightRule r; legal = r.legalDestinations(board, *source); break; }
        case Kind::King:   { KingRule r;   legal = r.legalDestinations(board, *source); break; }
        case Kind::Pawn:   { PawnRule r;   legal = r.legalDestinations(board, *source); break; }

        default:
            return MoveValidation{false, "illegal_piece_move"};
    }

    for (const Position& p : legal) {
        if (p == to) {
            return MoveValidation{true, "ok"};
        }
    }

    return MoveValidation{false, "illegal_piece_move"};
}

