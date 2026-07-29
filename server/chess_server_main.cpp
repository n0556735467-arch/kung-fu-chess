#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXNetSystem.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <mutex>

#include "game_session.hpp"
#include "bus.hpp"
#include "../engine/game_engine.hpp"
#include "../io/board_parser.hpp"
#include "../io/board_printer.hpp"

int main() {
    ix::initNetSystem();

    std::istringstream input(R"(bR bN bB bQ bK bB bN bR
bP bP bP bP bP bP bP bP
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
wP wP wP wP wP wP wP wP
wR wN wB wQ wK wB wN wR
)");
    Board board = parseBoard(input);

    Bus bus;
    GameEngine engine(board, &bus);
    GameSession session(engine);

    std::mutex clientsMutex;
    std::unordered_map<std::string, ix::WebSocket*> clients;

    auto broadcastBoard = [&]() {
        std::ostringstream out;
        printBoard(engine.getBoard(), out);
        std::string text = out.str();

        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& pair : clients) {
            pair.second->send(text);
        }
    };

    bus.subscribe(EventType::MoveLogged, [&broadcastBoard](const BusEvent&) {
        broadcastBoard();
    });
    bus.subscribe(EventType::GameEnded, [&broadcastBoard](const BusEvent&) {
        broadcastBoard();
    });

    ix::WebSocketServer server(8080, "0.0.0.0");

    server.setOnClientMessageCallback(
        [&](std::shared_ptr<ix::ConnectionState> connectionState,
            ix::WebSocket& webSocket,
            const ix::WebSocketMessagePtr& msg) {
            std::string connectionId = connectionState->getId();

            if (msg->type == ix::WebSocketMessageType::Open) {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clients[connectionId] = &webSocket;
                std::cout << "Client connected: " << connectionId << std::endl;
            } else if (msg->type == ix::WebSocketMessageType::Close) {
                {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    clients.erase(connectionId);
                }
                session.handleDisconnect(connectionId);
                std::cout << "Client disconnected: " << connectionId << std::endl;
            } else if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "Received from " << connectionId << ": " << msg->str << std::endl;
                session.handleCommand(connectionId, msg->str);
            }
        }
    );

    auto res = server.listen();
    if (!res.first) {
        std::cerr << "Failed to listen: " << res.second << std::endl;
        return 1;
    }

    server.start();
    std::cout << "Kung Fu Chess server running on ws://localhost:8080" << std::endl;
    server.wait();

    ix::uninitNetSystem();
    return 0;
}