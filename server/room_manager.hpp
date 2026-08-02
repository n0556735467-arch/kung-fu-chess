#ifndef ROOM_MANAGER_HPP
#define ROOM_MANAGER_HPP
#include <string>
#include <unordered_map>
#include <memory>
#include "room.hpp"

class RoomManager {
public:
    std::string createRoom();
    bool hasRoom(const std::string& roomId) const;
    Room* getRoom(const std::string& roomId);

private:
    std::unordered_map<std::string, std::unique_ptr<Room>> rooms;
};

#endif