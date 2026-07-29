#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <iostream>
#include <string>

int main() {
    ix::initNetSystem();

    ix::WebSocket webSocket;
    webSocket.setUrl("ws://localhost:8080");

    webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            std::cout << "Got: " << msg->str << std::endl;
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            std::cout << "Connected to server." << std::endl;
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
        }
    });

    webSocket.start();

    std::string line;
    std::cout << "Type a message and press Enter (or 'quit' to exit):" << std::endl;
    while (std::getline(std::cin, line)) {
        if (line == "quit") {
            break;
        }
        webSocket.send(line);
    }

    webSocket.stop();
    ix::uninitNetSystem();
    return 0;
}