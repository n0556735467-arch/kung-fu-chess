// placeholder header for game engine
// engine/game_engine.hpp
#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP
#include <optional>
#include "board.hpp"
#include "real_time_arbiter.hpp"
#include "rule_engine.hpp"

class GameEngine {
public:
    GameEngine(Board board);

    const Piece* pieceAt(Position pos) const;
    void requestMove(Position from, Position to);
    void wait(int ms);
    const Board& getBoard() const;

    std::optional<Position> getLastMoveFrom() const;
    std::optional<Position> getLastMoveTo() const;

private:
    Board board;
    RealTimeArbiter arbiter;
    RuleEngine ruleEngine;
    std::optional<Position> lastMoveFrom;
    std::optional<Position> lastMoveTo;
};

#endif