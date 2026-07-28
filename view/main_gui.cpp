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
    if (event != cv::EVENT_LBUTTONDOWN) {
        return;
    }
    Controller* controller = static_cast<Controller*>(userdata);
    controller->click(x, y);
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

        cv::namedWindow("Image");
        cv::setMouseCallback("Image", onMouse, &controller);

        auto lastFrameTime = std::chrono::steady_clock::now();

        while (!engine.isGameOver()) {
            auto now = std::chrono::steady_clock::now();
            int elapsedMs = static_cast<int>(
                std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count());
            lastFrameTime = now;

            engine.wait(elapsedMs);

                std::vector<Position> highlights;
    if (controller.hasSelection()) {
        highlights = engine.legalDestinationsFrom(controller.selectedPosition());
    }
            int key = renderer.render(engine.snapshot(), imageView, elapsedMs, highlights);
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