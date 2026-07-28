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

bool GameEngine::requestJump(Position at) {
    if (gameOver) {
        return false;
    }
    return arbiter.startJump(at, board);
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

GameSnapshot GameEngine::snapshot() const {
    GameSnapshot snap;
    snap.rows = board.rows;
    snap.cols = board.cols;
    snap.gameOver = gameOver;

    for (const Piece& p : board.getPieces()) {
        snap.pieces.push_back(PieceSnapshot{p.id, p.color, p.kind, p.state, p.cell});
    }

    return snap;
}