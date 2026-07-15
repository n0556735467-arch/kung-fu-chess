#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Position.h"

TEST_CASE("Two positions with same row and col are equal") {
    Position p1(3, 5);
    Position p2(3, 5);
    CHECK(p1 == p2);
}

TEST_CASE("Two positions with different col are not equal") {
    Position p1(3, 5);
    Position p2(3, 6);
    CHECK_FALSE(p1 == p2);
}

TEST_CASE("Two positions with different row are not equal") {
    Position p1(3, 5);
    Position p2(3, 6);
    CHECK_FALSE(p1 == p2);
}