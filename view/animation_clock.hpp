#ifndef ANIMATION_CLOCK_HPP
#define ANIMATION_CLOCK_HPP
#include "animation_config.hpp"

class AnimationClock {
public:
    explicit AnimationClock(AnimationConfig config);

    void advance(int ms);
    int currentFrame() const;
    bool isFinished() const;

private:
    AnimationConfig config;
    int elapsedMs = 0;
};

#endif