#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/room.hpp"

TEST_CASE("A new Room starts with a full standard board") {
    Room room;
    CHECK(room.getEngine().getBoard().getPieces().size() == 32);
}

TEST_CASE("Two different Room instances have independent game state") {
    Room roomA;
    Room roomB;

    roomA.getSession().handleCommand("conn1", "wP e2-e4");

    CHECK(roomA.getEngine().getMoveLog().size() == 1);
    CHECK(roomB.getEngine().getMoveLog().size() == 0);
}