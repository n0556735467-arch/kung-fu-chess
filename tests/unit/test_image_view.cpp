#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../view/image_view.hpp"
#include <unordered_map>

namespace {

class FakeConfigProvider : public AnimationConfigProvider {
public:
    std::unordered_map<std::string, AnimationConfig> configs;
    AnimationConfig configFor(const std::string& stateName) const override {
        return configs.at(stateName);
    }
};

FakeConfigProvider makeProvider() {
    FakeConfigProvider p;

    AnimationConfig idle;
    idle.framesPerSec = 4; idle.frameCount = 5; idle.isLoop = true;
    p.configs["idle"] = idle;

    AnimationConfig move;
    move.framesPerSec = 8; move.frameCount = 5; move.isLoop = false;
    move.nextStateWhenFinished = "long_rest";
    p.configs["move"] = move;

    AnimationConfig longRest;
    longRest.framesPerSec = 4; longRest.frameCount = 5; longRest.isLoop = false;
    longRest.nextStateWhenFinished = "idle";
    p.configs["long_rest"] = longRest;

    return p;
}

}

TEST_CASE("First update starts the triggered state") {
    FakeConfigProvider provider = makeProvider();
    ImageView view;

    view.update(1, provider, "idle", 0);

    CHECK(view.currentStateName(1) == "idle");
    CHECK(view.currentFrame(1) == 0);
}

TEST_CASE("A move trigger interrupts idle immediately") {
    FakeConfigProvider provider = makeProvider();
    ImageView view;

    view.update(1, provider, "idle", 500);
    view.update(1, provider, "move", 0);

    CHECK(view.currentStateName(1) == "move");
    CHECK(view.currentFrame(1) == 0);
}

TEST_CASE("Finished non-looping state auto-transitions to next_state_when_finished") {
    FakeConfigProvider provider = makeProvider();
    ImageView view;

    view.update(1, provider, "move", 0);
    view.update(1, provider, "idle", 625);

    CHECK(view.currentStateName(1) == "long_rest");
}

TEST_CASE("Two different pieces can use two different providers independently") {
    FakeConfigProvider providerA = makeProvider();

    FakeConfigProvider providerB;
    AnimationConfig fastIdle;
    fastIdle.framesPerSec = 20; fastIdle.frameCount = 5; fastIdle.isLoop = true;
    providerB.configs["idle"] = fastIdle;

    ImageView view;
    view.update(1, providerA, "idle", 250);   // provider A: 4fps -> frame 1
    view.update(2, providerB, "idle", 250);   // provider B: 20fps -> frame 5%5=0

    CHECK(view.currentFrame(1) == 1);
    CHECK(view.currentFrame(2) == 0);
}