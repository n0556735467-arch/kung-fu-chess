#include "player_registry.hpp"

void PlayerRegistry::setName(const std::string& connectionId, const std::string& name) {
    names[connectionId] = name;
}

std::string PlayerRegistry::getName(const std::string& connectionId) const {
    auto it = names.find(connectionId);
    if (it == names.end()) {
        return "Player";
    }
    return it->second;
}

void PlayerRegistry::release(const std::string& connectionId) {
    names.erase(connectionId);
}