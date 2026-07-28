#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP
#include <optional>
#include "../model/board.hpp"
#include "../realtime/real_time_arbiter.hpp"
#include "../rules/rule_engine.hpp"
#include "game_snapshot.hpp"
#include <vector>
#include "move_log_entry.hpp"

class GameEngine {
public:
    int whiteScore() const;
    int blackScore() const;
    
    GameEngine(Board board);

    const Piece* pieceAt(Position pos) const;
    void requestMove(Position from, Position to);
    bool requestJump(Position at);

    void wait(int ms);
    const Board& getBoard() const;
    bool isGameOver() const;

    const std::vector<MoveLogEntry>& getMoveLog() const;


    std::optional<Position> getLastMoveFrom() const;
    std::optional<Position> getLastMoveTo() const;

    std::vector<Position> legalDestinationsFrom(Position from) const;

    GameSnapshot snapshot() const;

private:
    int whiteScoreValue = 0;
    int blackScoreValue = 0;
    static int pieceValue(Kind kind);

    Board board;
    RealTimeArbiter arbiter;
    RuleEngine ruleEngine;
    bool gameOver = false;
    std::optional<Position> lastMoveFrom;
    std::optional<Position> lastMoveTo;

    int elapsedTotalMs = 0;
    std::vector<MoveLogEntry> moveLog;
    static std::string squareName(Position pos, int boardRows);
};

#endif