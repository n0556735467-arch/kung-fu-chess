#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/player_registry.hpp"

TEST_CASE("A registered name can be retrieved by connection id") {
    PlayerRegistry registry;
    registry.setName("conn1", "Dana");
    CHECK(registry.getName("conn1") == "Dana");
}

TEST_CASE("An unregistered connection returns a default placeholder") {
    PlayerRegistry registry;
    CHECK(registry.getName("unknown") == "Player");
}

TEST_CASE("release removes the stored name") {
    PlayerRegistry registry;
    registry.setName("conn1", "Dana");
    registry.release("conn1");
    CHECK(registry.getName("conn1") == "Player");
}