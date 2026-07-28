#ifndef RULE_ENGINE_HPP
#define RULE_ENGINE_HPP
#include <string>
#include <vector>
#include "../model/board.hpp"

struct MoveValidation {
    bool isValid;
    std::string reason;
};

class RuleEngine {
public:
    MoveValidation validateMove(const Board& board, Position from, Position to) const;
    std::vector<Position> legalDestinations(const Board& board, Position from) const;
};

#endif