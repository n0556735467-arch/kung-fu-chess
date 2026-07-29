#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXNetSystem.h>
#include <iostream>

int main() {
    ix::initNetSystem();

    ix::WebSocketServer server(8080, "0.0.0.0");

    server.setOnClientMessageCallback(
        [](std::shared_ptr<ix::ConnectionState> connectionState,
           ix::WebSocket& webSocket,
           const ix::WebSocketMessagePtr& msg) {
            if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "Received: " << msg->str << std::endl;
                webSocket.send("Echo: " + msg->str);
            }
        }
    );

    auto res = server.listen();
    if (!res.first) {
        std::cerr << "Failed to listen: " << res.second << std::endl;
        return 1;
    }

    server.start();
    std::cout << "Server running on ws://localhost:8080" << std::endl;
    server.wait();

    ix::uninitNetSystem();
    return 0;
}