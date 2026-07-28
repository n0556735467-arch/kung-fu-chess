#include "json_animation_config_provider.hpp"
#include <opencv2/core/persistence.hpp>
#include <filesystem>
#include <stdexcept>

JsonAnimationConfigProvider::JsonAnimationConfigProvider(std::string pieceBasePath)
    : pieceBasePath(std::move(pieceBasePath)) {}

AnimationConfig JsonAnimationConfigProvider::configFor(const std::string& stateName) const {
    namespace fs = std::filesystem;
    fs::path stateDir = fs::path(pieceBasePath) / "states" / stateName;

    cv::FileStorage fs_(  (stateDir / "config.json").string(), cv::FileStorage::READ);
    if (!fs_.isOpened()) {
        throw std::runtime_error("Cannot open config.json at: " + (stateDir / "config.json").string());
    }

    AnimationConfig config;
    fs_["graphics"]["frames_per_sec"] >> config.framesPerSec;

    int loopFlag = 0;
    fs_["graphics"]["is_loop"] >> loopFlag;
    config.isLoop = (loopFlag != 0);

    std::string nextState;
    fs_["physics"]["next_state_when_finished"] >> nextState;
    config.nextStateWhenFinished = nextState;

    int frameCount = 0;
    for (const auto& entry : fs::directory_iterator(stateDir / "sprites")) {
        if (entry.path().extension() == ".png") {
            frameCount++;
        }
    }
    config.frameCount = frameCount;

    return config;
}