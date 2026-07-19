// placeholder header for board mapper
#ifndef BOARD_MAPPER_HPP
#define BOARD_MAPPER_HPP
#include <optional>
#include "position.hpp"

std::optional<Position> pixelToCell(int x, int y, int rows, int cols);

#endif