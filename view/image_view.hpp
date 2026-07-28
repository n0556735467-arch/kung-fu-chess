#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP
#include <string>
#include <unordered_map>
#include "animation_clock.hpp"
#include "animation_config_provider.hpp"

class ImageView {
public:
    void update(int pieceId, const AnimationConfigProvider& provider,
                const std::string& triggeredState, int ms);

    std::string currentStateName(int pieceId) const;
    int currentFrame(int pieceId) const;

private:
    struct PerPieceAnim {
        std::string stateName;
        AnimationClock clock;
    };

    std::unordered_map<int, PerPieceAnim> animations;

    void startState(int pieceId, const AnimationConfigProvider& provider,
                     const std::string& stateName);
};

#endif