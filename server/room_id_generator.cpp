#include "room_id_generator.hpp"
#include <random>

std::string generateRoomId() {
    static const std::string charset = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, static_cast<int>(charset.size()) - 1);

    std::string id;
    for (int i = 0; i < 6; i++) {
        id += charset[dist(gen)];
    }
    return id;
}