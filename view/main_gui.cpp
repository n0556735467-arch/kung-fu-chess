#include "../view/renderer.hpp"
#include "../view/image_view.hpp"
#include "../io/board_parser.hpp"
#include "../engine/game_engine.hpp"
#include <sstream>
#include <iostream>

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

        Renderer renderer;
        ImageView imageView;

        renderer.render(engine.snapshot(), imageView, 0);   // elapsedMs=0: פריים ראשון בלבד
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}