#ifndef JSON_ANIMATION_CONFIG_PROVIDER_HPP
#define JSON_ANIMATION_CONFIG_PROVIDER_HPP
#include <string>
#include "animation_config_provider.hpp"

class JsonAnimationConfigProvider : public AnimationConfigProvider {
public:
    explicit JsonAnimationConfigProvider(std::string pieceBasePath);
    AnimationConfig configFor(const std::string& stateName) const override;

private:
    std::string pieceBasePath;   // לדוגמה: ".../assets/pieces_mine/wQ"
};

#endif