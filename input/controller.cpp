#include "controller.hpp"
#include "board_mapper.hpp"

Controller::Controller(GameEngine& engine) : engine(engine) {
}

bool Controller::hasSelection() const {
    return selected.has_value();
}

Position Controller::selectedPosition() const {
    return selected.value();
}

void Controller::click(int x, int y) {
    const Board& board = engine.getBoard();
    std::optional<Position> pos = pixelToCell(x, y, board.rows, board.cols);

    if (!hasSelection()) {
        if (!pos.has_value()) {
            return;
        }
        if (engine.pieceAt(pos.value()) != nullptr) {
            selected = pos;
        }
    } else {
        if (!pos.has_value()) {
            selected.reset();
            return;
        }

        const Piece* clicked = engine.pieceAt(pos.value());
        const Piece* current = engine.pieceAt(selected.value());

        if (clicked != nullptr && current != nullptr && clicked->color == current->color) {
            selected = pos;
        } else {
            engine.requestMove(selected.value(), pos.value());
            selected.reset();
        }
    }
}