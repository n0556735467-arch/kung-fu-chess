// placeholder C++ file for board mapper (empty)
#include "board_mapper.hpp"

std::optional<Position> pixelToCell(int x, int y, int rows, int cols) {
    if (x < 0 || y < 0) {
        return std::nullopt;
    }

    int col = x / 100;
    int row = y / 100;

    if (row >= rows || col >= cols) {
        return std::nullopt;
    }

    return Position(row, col);
}