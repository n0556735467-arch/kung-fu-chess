#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../server/room_id_generator.hpp"
#include <set>

TEST_CASE("Generated room id is 6 characters long") {
    std::string id = generateRoomId();
    CHECK(id.size() == 6);
}

TEST_CASE("Room ids do not contain visually confusing characters") {
    std::string id = generateRoomId();
    for (char c : id) {
        CHECK(c != 'I');
        CHECK(c != 'O');
        CHECK(c != '0');
        CHECK(c != '1');
    }
}

TEST_CASE("Repeated calls produce different ids (probabilistically)") {
    std::set<std::string> ids;
    for (int i = 0; i < 20; i++) {
        ids.insert(generateRoomId());
    }
    CHECK(ids.size() > 1);
}