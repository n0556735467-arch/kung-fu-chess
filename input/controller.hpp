#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include <optional>
#include "../engine/game_engine.hpp"

class Controller {
public:
    Controller(GameEngine& engine);

    void click(int x, int y);
    bool hasSelection() const;
    Position selectedPosition() const;

    void rightClick(int x, int y);

private:
    GameEngine& engine;
    std::optional<Position> selected;
};

#endif