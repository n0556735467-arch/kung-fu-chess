#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <string>
#include <unordered_map>
#include "img.hpp"
#include "image_view.hpp"
#include "json_animation_config_provider.hpp"
#include "../model/board.hpp"
#include "../engine/game_snapshot.hpp"

class Renderer {
public:
    void render(const GameSnapshot& snapshot, ImageView& imageView, int elapsedMs);

private:
    void drawPiece(Img& boardImg, const PieceSnapshot& piece,
                    ImageView& imageView, int elapsedMs);

    const AnimationConfigProvider& providerFor(const std::string& pieceCode);

    static std::string pieceCode(Color color, Kind kind);
    static std::string triggeredStateFor(PieceState state);

    std::unordered_map<std::string, JsonAnimationConfigProvider> providers;
};

#endif