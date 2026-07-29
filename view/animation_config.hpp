#ifndef ANIMATION_CONFIG_HPP
#define ANIMATION_CONFIG_HPP
#include <string>

struct AnimationConfig {
    int framesPerSec = 1;
    bool isLoop = true;
    int frameCount = 1;
    std::string nextStateWhenFinished;   
};

#endif