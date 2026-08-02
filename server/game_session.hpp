#ifndef GAME_SESSION_HPP
#define GAME_SESSION_HPP
#include <string>
#include "role_assigner.hpp"
#include "../engine/game_engine.hpp"
#include "player_registry.hpp"

class GameSession {
public:
    explicit GameSession(GameEngine& engine);

    void handleCommand(const std::string& connectionId, const std::string& text);
    void handleDisconnect(const std::string& connectionId);
    ClientRole getRole(const std::string& connectionId);
    std::string getWhiteName() const;
    std::string getBlackName() const;

private:
    GameEngine& engine;
    RoleAssigner roleAssigner;
    PlayerRegistry playerRegistry;
    std::string whiteConnectionId;
    std::string blackConnectionId;
};

#endif