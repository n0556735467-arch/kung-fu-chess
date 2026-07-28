#ifndef GAME_SNAPSHOT_HPP
#define GAME_SNAPSHOT_HPP
#include <vector>
#include "../model/piece.hpp"
#include "../model/position.hpp"

struct PieceSnapshot {
    int id;
    Color color;
    Kind kind;
    PieceState state;
    Position cell;
};

struct GameSnapshot {
    int rows;
    int cols;
    std::vector<PieceSnapshot> pieces;
    bool gameOver;
};

#endif