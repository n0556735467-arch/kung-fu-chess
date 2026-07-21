#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../input/board_mapper.hpp"

TEST_CASE("x=0..99 maps to column 0") {
    auto result = pixelToCell(50, 50, 8, 8);
    REQUIRE(result.has_value());
    CHECK(result->col == 0);
}

TEST_CASE("x=100..199 maps to column 1") {
    auto result = pixelToCell(150, 50, 8, 8);
    REQUIRE(result.has_value());
    CHECK(result->col == 1);
}

TEST_CASE("y=100..199 maps to row 1") {
    auto result = pixelToCell(50, 150, 8, 8);
    REQUIRE(result.has_value());
    CHECK(result->row == 1);
}

TEST_CASE("Outside click is rejected by BoardMapper") {
    auto result = pixelToCell(-10, 50, 8, 8);
    CHECK_FALSE(result.has_value());

    auto result2 = pixelToCell(50, 900, 8, 8);
    CHECK_FALSE(result2.has_value());
}