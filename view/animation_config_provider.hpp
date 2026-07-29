#ifndef ANIMATION_CONFIG_PROVIDER_HPP
#define ANIMATION_CONFIG_PROVIDER_HPP
#include <string>
#include "animation_config.hpp"

class AnimationConfigProvider {
public:
    virtual ~AnimationConfigProvider() = default;
    virtual AnimationConfig configFor(const std::string& stateName) const = 0;
};

#endif