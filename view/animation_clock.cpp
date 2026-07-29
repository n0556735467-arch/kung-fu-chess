#include "animation_clock.hpp"

AnimationClock::AnimationClock(AnimationConfig config) : config(config) {}

void AnimationClock::advance(int ms) {
    elapsedMs += ms;
}

int AnimationClock::currentFrame() const {
    int frameDurationMs = 1000 / config.framesPerSec;
    int index = elapsedMs / frameDurationMs;

    if (config.isLoop) {
        return index % config.frameCount;
    }
    if (index >= config.frameCount) {
        return config.frameCount - 1;
    }
    return index;
}

bool AnimationClock::isFinished() const {
    if (config.isLoop) {
        return false;
    }
    int frameDurationMs = 1000 / config.framesPerSec;
    return elapsedMs >= frameDurationMs * config.frameCount;
}