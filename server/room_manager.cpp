#include "room_manager.hpp"
#include "room_id_generator.hpp"

std::string RoomManager::createRoom() {
    std::string id;
    do {
        id = generateRoomId();
    } while (rooms.find(id) != rooms.end());

    rooms[id] = std::make_unique<Room>();
    return id;
}

bool RoomManager::hasRoom(const std::string& roomId) const {
    return rooms.find(roomId) != rooms.end();
}

Room* RoomManager::getRoom(const std::string& roomId) {
    auto it = rooms.find(roomId);
    if (it == rooms.end()) {
        return nullptr;
    }
    return it->second.get();
}