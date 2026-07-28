#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <filesystem>
#include <fstream>
#include "../../view/json_animation_config_provider.hpp"

namespace {

std::filesystem::path makeFixtureState(const std::string& json, int spriteCount) {
    auto dir = std::filesystem::temp_directory_path() / "kfc_test_piece" / "states" / "move";
    std::filesystem::create_directories(dir / "sprites");

    std::ofstream(dir / "config.json") << json;

    for (int i = 1; i <= spriteCount; ++i) {
        std::ofstream(dir / "sprites" / (std::to_string(i) + ".png")) << "fake";
    }

    return dir.parent_path().parent_path();  // .../kfc_test_piece
}

}

TEST_CASE("Loads frames_per_sec, is_loop and next_state_when_finished from json") {
    auto basePath = makeFixtureState(
        R"({
            "physics": { "speed_m_per_sec": 1.5, "next_state_when_finished": "long_rest" },
            "graphics": { "frames_per_sec": 8, "is_loop": false }
        })",
        5
    );

    JsonAnimationConfigProvider provider(basePath.string());
    AnimationConfig config = provider.configFor("move");

    CHECK(config.framesPerSec == 8);
    CHECK(config.isLoop == false);
    CHECK(config.nextStateWhenFinished == "long_rest");
    CHECK(config.frameCount == 5);
}