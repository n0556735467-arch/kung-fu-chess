#include "../view/renderer.hpp"
#include "../view/image_view.hpp"
#include "../io/board_parser.hpp"
#include "../engine/game_engine.hpp"
#include "../input/controller.hpp"
#include <opencv2/highgui.hpp>
#include <sstream>
#include <iostream>
#include <chrono>

namespace {

void onMouse(int event, int x, int y, int flags, void* userdata) {
    Controller* controller = static_cast<Controller*>(userdata);
    if (event == cv::EVENT_LBUTTONDOWN) {
        controller->click(x, y);
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        controller->rightClick(x, y);
    }
}

}

int main()
{
    try {
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
        GameEngine engine(board);
        Controller controller(engine);

        Renderer renderer;
        ImageView imageView;

        std::string whiteName, blackName;
std::cout << "Enter White player name: ";
std::getline(std::cin, whiteName);
std::cout << "Enter Black player name: ";
std::getline(std::cin, blackName);

if (whiteName.empty()) whiteName = "White";
if (blackName.empty()) blackName = "Black";

        cv::namedWindow("Image");
        cv::setMouseCallback("Image", onMouse, &controller);

        auto lastFrameTime = std::chrono::steady_clock::now();


        while (true) {
    auto now = std::chrono::steady_clock::now();
    int elapsedMs = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count());
    lastFrameTime = now;

    if (!engine.isGameOver()) {
        engine.wait(elapsedMs);
    }

    std::vector<Position> highlights;
    if (controller.hasSelection() && !engine.isGameOver()) {
        highlights = engine.legalDestinationsFrom(controller.selectedPosition());
    }

    std::string gameOverMessage;
    if (engine.isGameOver()) {
        Color winner = Color::White;
        for (const PieceSnapshot& p : engine.snapshot().pieces) {
            if (p.kind == Kind::King) {
                winner = p.color;
                break;
            }
        }
    std::string winnerName = (winner == Color::White) ? whiteName : blackName;
    gameOverMessage = winnerName + " WINS";
    }

    int key = renderer.render(engine.snapshot(), imageView, elapsedMs, highlights, gameOverMessage, whiteName, blackName);
    if (key == 27) {
        break;
    }
}
        

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}