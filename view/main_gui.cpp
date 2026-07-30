#include "../view/renderer.hpp"
#include "../view/image_view.hpp"
#include "../input/board_mapper.hpp"
#include "../server/snapshot_codec.hpp"
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <chrono>
#include <mutex>
#include <optional>

namespace {

std::string pieceCode(Color color, Kind kind) {
    std::string code;
    code += (color == Color::White ? 'w' : 'b');
    switch (kind) {
        case Kind::King:   code += 'K'; break;
        case Kind::Queen:  code += 'Q'; break;
        case Kind::Rook:   code += 'R'; break;
        case Kind::Bishop: code += 'B'; break;
        case Kind::Knight: code += 'N'; break;
        case Kind::Pawn:   code += 'P'; break;
    }
    return code;
}

std::string squareName(Position pos, int boardRows) {
    char file = 'a' + pos.col;
    int rank = boardRows - pos.row;
    return std::string(1, file) + std::to_string(rank);
}

struct SharedState {
    std::mutex mutex;
    GameSnapshot snapshot;
    bool hasSnapshot = false;
};

struct MouseState {
    SharedState* shared;
    ix::WebSocket* socket;
    std::optional<Position> selected;
};

void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event != cv::EVENT_LBUTTONDOWN) {
        return;
    }
    MouseState* state = static_cast<MouseState*>(userdata);

    GameSnapshot snapshotCopy;
    {
        std::lock_guard<std::mutex> lock(state->shared->mutex);
        if (!state->shared->hasSnapshot) {
            return;
        }
        snapshotCopy = state->shared->snapshot;
    }

    std::optional<Position> clicked = pixelToCell(x, y, snapshotCopy.rows, snapshotCopy.cols);
    if (!clicked.has_value()) {
        state->selected.reset();
        return;
    }

    const PieceSnapshot* clickedPiece = nullptr;
    for (const PieceSnapshot& p : snapshotCopy.pieces) {
        if (p.cell == clicked.value()) {
            clickedPiece = &p;
            break;
        }
    }

    if (!state->selected.has_value()) {
        if (clickedPiece != nullptr) {
            state->selected = clicked;
        }
        return;
    }

    const PieceSnapshot* selectedPiece = nullptr;
    for (const PieceSnapshot& p : snapshotCopy.pieces) {
        if (p.cell == state->selected.value()) {
            selectedPiece = &p;
            break;
        }
    }

    if (selectedPiece == nullptr) {
        state->selected.reset();
        return;
    }

    if (clickedPiece != nullptr && clickedPiece->color == selectedPiece->color) {
        state->selected = clicked;
        return;
    }

    std::string command = pieceCode(selectedPiece->color, selectedPiece->kind) + " "
        + squareName(state->selected.value(), snapshotCopy.rows) + "-"
        + squareName(clicked.value(), snapshotCopy.rows);

    state->socket->send(command);
    state->selected.reset();
}

}

int main() {
    ix::initNetSystem();

    std::string serverUrl = "ws://localhost:8080";
    std::cout << "Connecting to " << serverUrl << " ..." << std::endl;

    SharedState shared;

    ix::WebSocket socket;
    socket.setUrl(serverUrl);

    socket.setOnMessageCallback([&shared](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            GameSnapshot decoded = decodeSnapshot(msg->str);
            std::lock_guard<std::mutex> lock(shared.mutex);
            shared.snapshot = decoded;
            shared.hasSnapshot = true;
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            std::cout << "Connected to server." << std::endl;
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cerr << "Connection error: " << msg->errorInfo.reason << std::endl;
        }
    });

    socket.start();

    MouseState mouseState;
    mouseState.shared = &shared;
    mouseState.socket = &socket;

    Renderer renderer;
    ImageView imageView;

    cv::namedWindow("Image");
    cv::setMouseCallback("Image", onMouse, &mouseState);

    auto lastFrameTime = std::chrono::steady_clock::now();

    while (true) {
        auto now = std::chrono::steady_clock::now();
        int elapsedMs = static_cast<int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count());
        lastFrameTime = now;

        GameSnapshot snapshotCopy;
        bool has = false;
        {
            std::lock_guard<std::mutex> lock(shared.mutex);
            has = shared.hasSnapshot;
            if (has) {
                snapshotCopy = shared.snapshot;
            }
        }

        if (!has) {
            cv::waitKey(30);
            continue;
        }

        std::string gameOverMessage = snapshotCopy.gameOver ? "GAME OVER" : "";

        int key = renderer.render(snapshotCopy, imageView, elapsedMs, {}, gameOverMessage);
        if (key == 27) {
            break;
        }
    }

    socket.stop();
    ix::uninitNetSystem();
    return 0;
}