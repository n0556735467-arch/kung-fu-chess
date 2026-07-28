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

            const Piece* moved = board.pieceAt(from);
if (moved != nullptr) {
    std::string code;
    code += (moved->color == Color::White ? 'w' : 'b');
    switch (moved->kind) {
        case Kind::King:   code += 'K'; break;
        case Kind::Queen:  code += 'Q'; break;
        case Kind::Rook:   code += 'R'; break;
        case Kind::Bishop: code += 'B'; break;
        case Kind::Knight: code += 'N'; break;
        case Kind::Pawn:   code += 'P'; break;
    }
    std::string text = code + " " + squareName(from, board.rows) + "-" + squareName(to, board.rows);
    moveLog.push_back(MoveLogEntry{elapsedTotalMs, text});
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

int GameEngine::pieceValue(Kind kind) {
    switch (kind) {
        case Kind::Pawn:   return 1;
        case Kind::Knight: return 3;
        case Kind::Bishop: return 3;
        case Kind::Rook:   return 5;
        case Kind::Queen:  return 9;
        case Kind::King:   return 0;
    }
    return 0;
}

void GameEngine::wait(int ms) {
    elapsedTotalMs += ms;
    bool kingCaptured = arbiter.wait(ms, board);
    if (kingCaptured) {
        gameOver = true;
    }
        for (const CapturedPiece& cp : arbiter.consumeCaptures()) {
        int value = pieceValue(cp.kind);
        if (cp.color == Color::White) {
            blackScoreValue += value;
        } else {
            whiteScoreValue += value;
        }
    }
}

int GameEngine::whiteScore() const { return whiteScoreValue; }
int GameEngine::blackScore() const { return blackScoreValue; }

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

std::vector<Position> GameEngine::legalDestinationsFrom(Position from) const {
    return ruleEngine.legalDestinations(board, from);
}

GameSnapshot GameEngine::snapshot() const {
    GameSnapshot snap;
    snap.rows = board.rows;
    snap.cols = board.cols;
    snap.gameOver = gameOver;
    snap.whiteScore = whiteScoreValue;
    snap.blackScore = blackScoreValue;

    for (const Piece& p : board.getPieces()) {
        PieceSnapshot ps{p.id, p.color, p.kind, p.state, p.cell, arbiter.activeMotionFor(p.id)};
        snap.pieces.push_back(ps);
    }
    
    snap.moveLog = moveLog;
    return snap;
}

std::string GameEngine::squareName(Position pos, int boardRows) {
    char file = 'a' + pos.col;
    int rank = boardRows - pos.row;
    return std::string(1, file) + std::to_string(rank);
}

const std::vector<MoveLogEntry>& GameEngine::getMoveLog() const {
    return moveLog;
}