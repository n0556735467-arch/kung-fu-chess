#include "image_view.hpp"

void ImageView::startState(int pieceId, const AnimationConfigProvider& provider,
                            const std::string& stateName) {
    AnimationConfig cfg = provider.configFor(stateName);
    animations.insert_or_assign(pieceId, PerPieceAnim{stateName, AnimationClock(cfg)});
}

void ImageView::update(int pieceId, const AnimationConfigProvider& provider,
                        const std::string& triggeredState, int ms) {
    bool isForceTrigger = (triggeredState == "move" || triggeredState == "jump");

    auto it = animations.find(pieceId);
    if (it == animations.end()) {
        startState(pieceId, provider, triggeredState);
    } else if (isForceTrigger && it->second.stateName != triggeredState) {
        startState(pieceId, provider, triggeredState);
    }

    it = animations.find(pieceId);
    it->second.clock.advance(ms);

    if (it->second.clock.isFinished()) {
        AnimationConfig cfg = provider.configFor(it->second.stateName);
        if (!cfg.nextStateWhenFinished.empty()) {
            startState(pieceId, provider, cfg.nextStateWhenFinished);
        }
    }
}

std::string ImageView::currentStateName(int pieceId) const {
    return animations.at(pieceId).stateName;
}

int ImageView::currentFrame(int pieceId) const {
    return animations.at(pieceId).clock.currentFrame();
}
