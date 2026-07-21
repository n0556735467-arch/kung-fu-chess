#ifndef JUMP_HPP
#define JUMP_HPP

struct Jump {
    int pieceId;
    int remainingMs;

    Jump(int pieceId, int durationMs);
};

#endif