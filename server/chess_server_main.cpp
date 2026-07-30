#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXNetSystem.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>

#include "game_session.hpp"
#include "bus.hpp"
#include "../engine/game_engine.hpp"
#include "../io/board_parser.hpp"
#include "snapshot_codec.hpp"

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
    std::recursive_mutex engineMutex;
    std::unordered_map<std::string, ix::WebSocket*> clients;
    std::string lastBroadcastText;

    auto broadcastBoard = [&]() {
        std::string text;
        {
            std::lock_guard<std::recursive_mutex> lock(engineMutex);
            text = encodeSnapshot(engine.snapshot());
        }

        if (text == lastBroadcastText) {
            return;
        }
        lastBroadcastText = text;

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
                {
                    std::lock_guard<std::recursive_mutex> lock(engineMutex);
                    session.handleDisconnect(connectionId);
                }
                std::cout << "Client disconnected: " << connectionId << std::endl;
            } else if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "Received from " << connectionId << ": " << msg->str << std::endl;
                {
                    std::lock_guard<std::recursive_mutex> lock(engineMutex);
                    session.handleCommand(connectionId, msg->str);
                }
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

    std::thread gameLoopThread([&]() {
        auto lastTick = std::chrono::steady_clock::now();
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            auto now = std::chrono::steady_clock::now();
            int elapsedMs = static_cast<int>(
                std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count());
            lastTick = now;

            {
                std::lock_guard<std::recursive_mutex> lock(engineMutex);
                engine.wait(elapsedMs);
            }
            broadcastBoard();
        }
    });
    gameLoopThread.detach();

    server.wait();

    ix::uninitNetSystem();
    return 0;
}