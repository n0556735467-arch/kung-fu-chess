#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXNetSystem.h>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>

#include "room.hpp"
#include "room_manager.hpp"
#include "bus.hpp"
#include "snapshot_codec.hpp"

int main() {
    ix::initNetSystem();

    RoomManager roomManager;
    std::recursive_mutex roomsMutex;

    std::mutex clientsMutex;
    std::unordered_map<std::string, ix::WebSocket*> clients;
    std::unordered_map<std::string, std::string> connectionToRoom;
    std::unordered_map<std::string, std::string> lastBroadcastText;
    std::unordered_map<std::string, std::string> lastBroadcastNames;

    auto sendRoomState = [&](const std::string& roomId, ix::WebSocket& socket) {
        Room* room = roomManager.getRoom(roomId);
        if (room == nullptr) {
            return;
        }
        std::string text = encodeSnapshot(room->getEngine().snapshot());
        socket.send(text);
    };

    auto sendJoinPackage = [&](const std::string& roomId, const std::string& connectionId, ix::WebSocket& socket) {
        Room* room = roomManager.getRoom(roomId);
        if (room == nullptr) {
            return;
        }

        ClientRole role = room->getSession().getRole(connectionId);
        std::string roleText = "ROLE ";
        roleText += (role == ClientRole::White ? "White" :
                     role == ClientRole::Black ? "Black" : "Observer");
        socket.send(roleText);

        std::string namesText = "NAMES " + room->getSession().getWhiteName() + "|"
            + room->getSession().getBlackName();
        socket.send(namesText);

        sendRoomState(roomId, socket);
    };

    auto broadcastRoom = [&](const std::string& roomId) {
        Room* room = roomManager.getRoom(roomId);
        if (room == nullptr) {
            return;
        }

        std::string namesText = "NAMES " + room->getSession().getWhiteName() + "|"
            + room->getSession().getBlackName();
        if (lastBroadcastNames[roomId] != namesText) {
            lastBroadcastNames[roomId] = namesText;
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (auto& pair : connectionToRoom) {
                if (pair.second == roomId && clients.count(pair.first)) {
                    clients[pair.first]->send(namesText);
                }
            }
        }

        std::string text = encodeSnapshot(room->getEngine().snapshot());
        if (lastBroadcastText[roomId] == text) {
            return;
        }
        lastBroadcastText[roomId] = text;

        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& pair : connectionToRoom) {
            if (pair.second == roomId && clients.count(pair.first)) {
                clients[pair.first]->send(text);
            }
        }
    };

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
                std::string roomId;
                ClientRole disconnectedRole = ClientRole::Observer;
                {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    clients.erase(connectionId);
                    auto it = connectionToRoom.find(connectionId);
                    if (it != connectionToRoom.end()) {
                        roomId = it->second;
                        connectionToRoom.erase(it);
                    }
                }
                if (!roomId.empty()) {
                    std::lock_guard<std::recursive_mutex> lock(roomsMutex);
                    Room* room = roomManager.getRoom(roomId);
                    if (room != nullptr) {
                        disconnectedRole = room->getSession().getRole(connectionId);
                        room->getSession().handleDisconnect(connectionId);
                    }
                }
                std::cout << "Client disconnected: " << connectionId << std::endl;
                if (disconnectedRole == ClientRole::White || disconnectedRole == ClientRole::Black) {
        std::string capturedRoomId = roomId;
        Color resigningColor = (disconnectedRole == ClientRole::White) ? Color::White : Color::Black;

        std::thread([&roomManager, &roomsMutex, capturedRoomId, resigningColor, broadcastRoom]() {
            std::this_thread::sleep_for(std::chrono::seconds(20));

            std::lock_guard<std::recursive_mutex> lock(roomsMutex);
            Room* room = roomManager.getRoom(capturedRoomId);
            if (room != nullptr && !room->getEngine().isGameOver()) {
                room->getEngine().resign(resigningColor);
                broadcastRoom(capturedRoomId);
            }
        }).detach();
    }
            } else if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "Received from " << connectionId << ": " << msg->str << std::endl;

                if (msg->str == "CREATE") {
                    std::string roomId;
                    {
                        std::lock_guard<std::recursive_mutex> lock(roomsMutex);
                        roomId = roomManager.createRoom();
                    }
                    {
                        std::lock_guard<std::mutex> lock(clientsMutex);
                        connectionToRoom[connectionId] = roomId;
                    }
                    webSocket.send("ROOMID " + roomId);
                    sendJoinPackage(roomId, connectionId, webSocket);

                } else if (msg->str.rfind("JOIN ", 0) == 0) {
                    std::string roomId = msg->str.substr(5);
                    bool exists;
                    {
                        std::lock_guard<std::recursive_mutex> lock(roomsMutex);
                        exists = roomManager.hasRoom(roomId);
                    }
                    if (!exists) {
                        webSocket.send("ERROR Room not found");
                        return;
                    }
                    {
                        std::lock_guard<std::mutex> lock(clientsMutex);
                        connectionToRoom[connectionId] = roomId;
                    }
                    webSocket.send("ROOMID " + roomId);
                    sendJoinPackage(roomId, connectionId, webSocket);

                } else {
                    std::string roomId;
                    {
                        std::lock_guard<std::mutex> lock(clientsMutex);
                        auto it = connectionToRoom.find(connectionId);
                        if (it != connectionToRoom.end()) {
                            roomId = it->second;
                        }
                    }
                    if (roomId.empty()) {
                        webSocket.send("ERROR Join or create a room first");
                        return;
                    }

                    Room* room;
                    {
                        std::lock_guard<std::recursive_mutex> lock(roomsMutex);
                        room = roomManager.getRoom(roomId);
                    }
                    if (room == nullptr) {
                        return;
                    }

                    room->getSession().handleCommand(connectionId, msg->str);
                    broadcastRoom(roomId);
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

            std::vector<std::string> roomIds;
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                for (auto& pair : connectionToRoom) {
                    roomIds.push_back(pair.second);
                }
            }

            for (const std::string& roomId : roomIds) {
                Room* room = roomManager.getRoom(roomId);
                if (room != nullptr) {
                    room->getEngine().wait(elapsedMs);
                }
            }
            for (const std::string& roomId : roomIds) {
                broadcastRoom(roomId);
            }
        }
    });
    gameLoopThread.detach();

    server.wait();

    ix::uninitNetSystem();
    return 0;
}