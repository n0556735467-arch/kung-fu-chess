#ifndef POSITION_HPP
#define POSITION_HPP

struct Position {
    int row;
    int col;

    Position(int row, int col);
    bool operator==(const Position& other) const;
};

#endif