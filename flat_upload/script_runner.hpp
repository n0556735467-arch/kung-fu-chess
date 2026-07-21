#ifndef SCRIPT_RUNNER_HPP
#define SCRIPT_RUNNER_HPP
#include <istream>
#include <ostream>
#include "game_engine.hpp"
#include "controller.hpp"

class ScriptRunner {
public:
    void run(std::istream& in, std::ostream& out, GameEngine& engine, Controller& controller);
};

#endif