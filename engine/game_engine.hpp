// placeholder header for game engine
#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP
#include <optional>
#include "../model/board.hpp"

class GameEngine {
public:
    GameEngine(Board board);

    const Piece* pieceAt(Position pos) const;
    void requestMove(Position from, Position to);
    const Board& getBoard() const;

    std::optional<Position> getLastMoveFrom() const;
    std::optional<Position> getLastMoveTo() const;

private:
    Board board;
    std::optional<Position> lastMoveFrom;
    std::optional<Position> lastMoveTo;
};

#endif