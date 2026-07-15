#ifndef POSITION_H
#define POSITION_H

struct Position {
    int row;
    int col;

    Position (int row, int col) {
        this->row = row;
        this->col = col;
    }

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

#endif