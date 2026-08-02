#include "../view/renderer.hpp"
#include "../view/image_view.hpp"
#include "../input/board_mapper.hpp"
#include "../server/snapshot_codec.hpp"
#include "../rules/rule_engine.hpp"
#include "../server/role_assigner.hpp"
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <chrono>
#include <mutex>
#include <optional>
#include <atomic>
#include <thread>

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

bool colorAllowed(Color c, ClientRole role) {
    return (role == ClientRole::White && c == Color::White) ||
           (role == ClientRole::Black && c == Color::Black);
}

struct SharedState {
    std::mutex mutex;
    GameSnapshot snapshot;
    bool hasSnapshot = false;
    ClientRole role = ClientRole::Observer;
    std::string whiteName = "White";
    std::string blackName = "Black";
};

struct MouseState {
    SharedState* shared;
    ix::WebSocket* socket;
    std::optional<Position> selected;
};

void onMouse(int event, int x, int y, int flags, void* userdata) {
    MouseState* state = static_cast<MouseState*>(userdata);

    if (event == cv::EVENT_RBUTTONDOWN) {
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
            return;
        }

        for (const PieceSnapshot& p : snapshotCopy.pieces) {
            if (p.cell == clicked.value()) {
                if (!colorAllowed(p.color, state->shared->role)) {
                    return;
                }
                std::string command = "JUMP " + pieceCode(p.color, p.kind) + " "
                    + squareName(clicked.value(), snapshotCopy.rows);
                state->socket->send(command);
                break;
            }
        }
        return;
    }

    if (event != cv::EVENT_LBUTTONDOWN) {
        return;
    }

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
        if (clickedPiece != nullptr && colorAllowed(clickedPiece->color, state->shared->role)) {
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
    std::atomic<bool> connected{false};

    ix::WebSocket socket;
    socket.setUrl(serverUrl);

    socket.setOnMessageCallback([&shared, &connected](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            if (msg->str.rfind("ROLE ", 0) == 0) {
                std::string roleStr = msg->str.substr(5);
                std::lock_guard<std::mutex> lock(shared.mutex);
                if (roleStr == "White") shared.role = ClientRole::White;
                else if (roleStr == "Black") shared.role = ClientRole::Black;
                else shared.role = ClientRole::Observer;
            } else if (msg->str.rfind("NAMES ", 0) == 0) {
                std::string namesStr = msg->str.substr(6);
                size_t sep = namesStr.find('|');
                if (sep != std::string::npos) {
                    std::lock_guard<std::mutex> lock(shared.mutex);
                    shared.whiteName = namesStr.substr(0, sep);
                    shared.blackName = namesStr.substr(sep + 1);
                }
            } else {
                GameSnapshot decoded = decodeSnapshot(msg->str);
                std::lock_guard<std::mutex> lock(shared.mutex);
                shared.snapshot = decoded;
                shared.hasSnapshot = true;
            }
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            std::cout << "Connected to server." << std::endl;
            connected = true;
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cerr << "Connection error: " << msg->errorInfo.reason << std::endl;
        }
    });

    socket.start();

    while (!connected) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::string playerName;
    std::cout << "Enter your name: ";
    std::getline(std::cin, playerName);
    if (playerName.empty()) {
        playerName = "Player";
    }
    socket.send("LOGIN " + playerName);

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

        Board localBoard(snapshotCopy.rows, snapshotCopy.cols);
        for (const PieceSnapshot& p : snapshotCopy.pieces) {
            localBoard.addPiece(Piece(p.id, p.color, p.kind, p.cell));
        }

        std::vector<Position> highlights;
        std::optional<Position> selectedCellForRender;
        if (mouseState.selected.has_value()) {
            selectedCellForRender = mouseState.selected;
            RuleEngine localRuleEngine;
            highlights = localRuleEngine.legalDestinations(localBoard, mouseState.selected.value());
        }

        std::string gameOverMessage = snapshotCopy.gameOver ? "GAME OVER" : "";

        std::string whiteNameCopy, blackNameCopy;
        {
            std::lock_guard<std::mutex> lock(shared.mutex);
            whiteNameCopy = shared.whiteName;
            blackNameCopy = shared.blackName;
        }

        int key = renderer.render(snapshotCopy, imageView, elapsedMs, highlights, gameOverMessage,
                                  whiteNameCopy, blackNameCopy, selectedCellForRender);
        if (key == 27) {
            break;
        }
    }

    socket.stop();
    ix::uninitNetSystem();
    return 0;
}