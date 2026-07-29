#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/bus.hpp"

TEST_CASE("A subscriber receives an event it subscribed to") {
    Bus bus;
    std::string received;

    bus.subscribe(EventType::PieceCaptured, [&received](const BusEvent& e) {
        received = e.payload;
    });

    bus.publish(BusEvent{EventType::PieceCaptured, "bP"});

    CHECK(received == "bP");
}

TEST_CASE("A subscriber does not receive events of a different type") {
    Bus bus;
    bool called = false;

    bus.subscribe(EventType::PieceCaptured, [&called](const BusEvent& e) {
        called = true;
    });

    bus.publish(BusEvent{EventType::MoveLogged, "wP e2-e4"});

    CHECK_FALSE(called);
}

TEST_CASE("Multiple subscribers to the same event type all get called") {
    Bus bus;
    int count = 0;

    bus.subscribe(EventType::GameEnded, [&count](const BusEvent& e) { count++; });
    bus.subscribe(EventType::GameEnded, [&count](const BusEvent& e) { count++; });

    bus.publish(BusEvent{EventType::GameEnded, "White wins"});

    CHECK(count == 2);
}