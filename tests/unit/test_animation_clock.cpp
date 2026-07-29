#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../view/animation_clock.hpp"

TEST_CASE("Looping clock cycles through frames and wraps around") {
    AnimationConfig config;
    config.framesPerSec = 4;   // 250ms per frame
    config.frameCount = 5;
    config.isLoop = true;

    AnimationClock clock(config);
    CHECK(clock.currentFrame() == 0);

    clock.advance(250);
    CHECK(clock.currentFrame() == 1);

    clock.advance(750);   // total 1000ms -> index 4
    CHECK(clock.currentFrame() == 4);

    clock.advance(250);   // total 1250ms -> index 5 -> wraps to 0
    CHECK(clock.currentFrame() == 0);
    CHECK(clock.isFinished() == false);
}

TEST_CASE("Non-looping clock stops on last frame and reports finished") {
    AnimationConfig config;
    config.framesPerSec = 8;   // 125ms per frame
    config.frameCount = 5;
    config.isLoop = false;

    AnimationClock clock(config);

    clock.advance(124);
    CHECK(clock.currentFrame() == 0);
    CHECK(clock.isFinished() == false);

    clock.advance(1);     // 125ms -> frame 1
    CHECK(clock.currentFrame() == 1);

    clock.advance(1000);  // הרבה מעבר ל-5 פריימים
    CHECK(clock.currentFrame() == 4);   // נתקע על הפריים האחרון
    CHECK(clock.isFinished() == true);
}