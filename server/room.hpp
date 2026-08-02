#ifndef ROOM_HPP
#define ROOM_HPP
#include <sstream>
#include "bus.hpp"
#include "game_session.hpp"
#include "../engine/game_engine.hpp"
#include "../io/board_parser.hpp"

class Room {
public:
    Room();

    GameSession& getSession();
    GameEngine& getEngine();

private:
    Board board;
    Bus bus;
    GameEngine engine;
    GameSession session;

    static Board createInitialBoard();
};

#endif