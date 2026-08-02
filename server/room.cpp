#include "room.hpp"

Board Room::createInitialBoard() {
    std::istringstream input(R"(bR bN bB bQ bK bB bN bR
bP bP bP bP bP bP bP bP
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
wP wP wP wP wP wP wP wP
wR wN wB wQ wK wB wN wR
)");
    return parseBoard(input);
}

Room::Room()
    : board(createInitialBoard()),
      engine(board, &bus),
      session(engine) {}

GameSession& Room::getSession() {
    return session;
}

GameEngine& Room::getEngine() {
    return engine;
}