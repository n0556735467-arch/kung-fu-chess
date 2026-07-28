#ifndef MOVE_LOG_ENTRY_HPP
#define MOVE_LOG_ENTRY_HPP
#include <string>

struct MoveLogEntry {
    int timeMs;
    std::string text;
};

#endif