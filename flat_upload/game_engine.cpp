#include "game_engine.hpp"

GameEngine::GameEngine(Board board) : board(board) {
}

const Piece* GameEngine::pieceAt(Position pos) const {
    return board.pieceAt(pos);
}

void GameEngine::requestMove(Position from, Position to) {
    if (gameOver) {
        return;
    }

    MoveValidation result = ruleEngine.validateMove(board, from, to);
    if (!result.isValid) {
        return;
    }

    bool started = arbiter.startMotion(from, to, board);
    if (!started) {
        return;
    }

    lastMoveFrom = from;
    lastMoveTo = to;
}

void GameEngine::wait(int ms) {
    bool kingCaptured = arbiter.wait(ms, board);
    if (kingCaptured) {
        gameOver = true;
    }
}

bool GameEngine::isGameOver() const {
    return gameOver;
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