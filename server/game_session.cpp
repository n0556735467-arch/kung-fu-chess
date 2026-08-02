#include "game_session.hpp"
#include "command_parser.hpp"

GameSession::GameSession(GameEngine& engine) : engine(engine) {}

void GameSession::handleCommand(const std::string& connectionId, const std::string& text) {
    ClientRole role = roleAssigner.assignRole(connectionId);
    
    if (text.rfind("LOGIN", 0) == 0) {
        std::string name = text.size() > 6 ? text.substr(6) : "Player";
        playerRegistry.setName(connectionId, name);
        if (role == ClientRole::White) {
            whiteConnectionId = connectionId;
        } else if (role == ClientRole::Black) {
            blackConnectionId = connectionId;
        }
        return;
    }

    if (role == ClientRole::Observer) {
        return;
    }

    if (text.rfind("JUMP", 0) == 0) {
        auto parsedJump = parseJumpCommand(text, engine.getBoard().rows);
        if (!parsedJump.has_value()) {
            return;
        }
        bool colorMatchesRole = (role == ClientRole::White && parsedJump->color == Color::White) ||
                                (role == ClientRole::Black && parsedJump->color == Color::Black);
        if (!colorMatchesRole) {
            return;
        }
        engine.requestJump(parsedJump->at);
        return;
    }

    auto parsed = parseCommand(text, engine.getBoard().rows);
    if (!parsed.has_value()) {
        return;
    }

    bool colorMatchesRole = (role == ClientRole::White && parsed->color == Color::White) ||
                            (role == ClientRole::Black && parsed->color == Color::Black);
    if (!colorMatchesRole) {
        return;
    }

    engine.requestMove(parsed->from, parsed->to);
}

ClientRole GameSession::getRole(const std::string& connectionId) {
    return roleAssigner.assignRole(connectionId);
}

void GameSession::handleDisconnect(const std::string& connectionId) {
    roleAssigner.release(connectionId);
}

std::string GameSession::getWhiteName() const {
    return whiteConnectionId.empty() ? "White" : playerRegistry.getName(whiteConnectionId);
}
std::string GameSession::getBlackName() const {
    return blackConnectionId.empty() ? "Black" : playerRegistry.getName(blackConnectionId);
}