#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP
#include <optional>
#include "../model/board.hpp"
#include "../realtime/real_time_arbiter.hpp"
#include "../rules/rule_engine.hpp"

class GameEngine {
public:
    GameEngine(Board board);

    const Piece* pieceAt(Position pos) const;
    void requestMove(Position from, Position to);
    bool requestJump(Position at);

    void wait(int ms);
    const Board& getBoard() const;
    bool isGameOver() const;

    std::optional<Position> getLastMoveFrom() const;
    std::optional<Position> getLastMoveTo() const;

private:
    Board board;
    RealTimeArbiter arbiter;
    RuleEngine ruleEngine;
    bool gameOver = false;
    std::optional<Position> lastMoveFrom;
    std::optional<Position> lastMoveTo;
};

#endif