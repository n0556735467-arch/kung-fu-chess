// placeholder header for piece rules
#ifndef PIECE_RULES_HPP
#define PIECE_RULES_HPP
#include <vector>
#include "board.hpp"

class PieceRule {
public:
    virtual std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const = 0;
    virtual ~PieceRule() = default;
};

class RookRule : public PieceRule {
public:
    std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const override;
};

class BishopRule : public PieceRule {
public:
    std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const override;
};

class QueenRule : public PieceRule {
public:
    std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const override;
};

class KnightRule : public PieceRule {
public:
    std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const override;
};

class KingRule : public PieceRule {
public:
    std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const override;
};

const PieceRule& getPieceRule(Kind kind);

class PawnRule : public PieceRule {
public:
    std::vector<Position> legalDestinations(const Board& board, const Piece& piece) const override;
};
#endif