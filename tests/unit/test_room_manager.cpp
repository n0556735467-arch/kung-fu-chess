#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/room_manager.hpp"

TEST_CASE("createRoom never returns an id that already exists") {
    RoomManager manager;

    std::string firstId = manager.createRoom();
    std::string secondId = manager.createRoom();

    CHECK(firstId != secondId);
    CHECK(manager.hasRoom(firstId));
    CHECK(manager.hasRoom(secondId));
}