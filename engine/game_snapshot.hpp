#ifndef GAME_SNAPSHOT_HPP
#define GAME_SNAPSHOT_HPP
#include <vector>
#include <optional>
#include "../model/piece.hpp"
#include "../model/position.hpp"
#include "../realtime/motion_snapshot.hpp"
#include "move_log_entry.hpp"

struct PieceSnapshot {
    int id;
    Color color;
    Kind kind;
    PieceState state;
    Position cell;
    std::optional<MotionSnapshot> motion;
};

struct GameSnapshot {
    int rows;
    int cols;
    std::vector<PieceSnapshot> pieces;
    bool gameOver;
    int whiteScore;
    int blackScore;
    std::vector<MoveLogEntry> moveLog;
    std::optional<Color> winner;
};

#endif