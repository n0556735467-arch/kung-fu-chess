#include <iostream>
#include "game_engine.hpp"

GameEngine::GameEngine(Board board) : board(board) {
}

const Piece* GameEngine::pieceAt(Position pos) const {
    return board.pieceAt(pos);
}

void GameEngine::requestMove(Position from, Position to) {
    MoveValidation result = ruleEngine.validateMove(board, from, to);
    std::cerr << "[DEBUG] requestMove from(" << from.row << "," << from.col
               << ") to(" << to.row << "," << to.col
               << ") valid=" << result.isValid << " reason=" << result.reason << "\n";

    if (!result.isValid) {
        return;
    }

    bool started = arbiter.startMotion(from, to, board);
    std::cerr << "[DEBUG] startMotion started=" << started << "\n";

    if (!started) {
        return;
    }

    lastMoveFrom = from;
    lastMoveTo = to;
}

void GameEngine::wait(int ms) {
    arbiter.wait(ms, board);
}

const Board& GameEngine::getBoard() const {
    return board;
}

std::optional<Position> GameEngine::getLastMoveFrom() const {
    return lastMoveFrom;
}

std::optional<Position> GameEngine::getLastMoveTo() const {
    return lastMoveTo;
}