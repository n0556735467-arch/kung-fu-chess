#ifndef SNAPSHOT_CODEC_HPP
#define SNAPSHOT_CODEC_HPP
#include <string>
#include "../engine/game_snapshot.hpp"

std::string encodeSnapshot(const GameSnapshot& snapshot);
GameSnapshot decodeSnapshot(const std::string& text);

#endif