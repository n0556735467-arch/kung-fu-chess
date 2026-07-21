#include "board_mapper.hpp"
#include "constants.hpp"

std::optional<Position> pixelToCell(int x, int y, int rows, int cols) {
    if (x < 0 || y < 0) {
        return std::nullopt;
    }

    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;

    if (row >= rows || col >= cols) {
        return std::nullopt;
    }

    return Position(row, col);
}