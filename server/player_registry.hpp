#ifndef PLAYER_REGISTRY_HPP
#define PLAYER_REGISTRY_HPP
#include <string>
#include <unordered_map>

class PlayerRegistry {
public:
    void setName(const std::string& connectionId, const std::string& name);
    std::string getName(const std::string& connectionId) const;
    void release(const std::string& connectionId);

private:
    std::unordered_map<std::string, std::string> names;
};

#endif