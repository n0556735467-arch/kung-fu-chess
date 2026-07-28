#include "renderer.hpp"
#include "../model/constants.hpp"
#include <opencv2/imgproc.hpp>

std::string Renderer::pieceCode(Color color, Kind kind) {
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

std::string Renderer::triggeredStateFor(PieceState state) {
    switch (state) {
        case PieceState::Moving:   return "move";
        case PieceState::Airborne: return "jump";
        default:                   return "idle";
    }
}

const AnimationConfigProvider& Renderer::providerFor(const std::string& code) {
    auto it = providers.find(code);
    if (it == providers.end()) {
        std::string basePath = std::string(ASSETS_DIR) + "/pieces_mine/" + code;
        it = providers.emplace(code, JsonAnimationConfigProvider(basePath)).first;
    }
    return it->second;
}




static void drawHighlight(cv::Mat& boardMat, Position cell) {
    int x = cell.col * CELL_SIZE;
    int y = cell.row * CELL_SIZE;
    cv::rectangle(boardMat, cv::Rect(x, y, CELL_SIZE, CELL_SIZE), cv::Scalar(0, 220, 0), 2);
}


Img& Renderer::cachedImage(const std::string& path, bool keepAspect) {
    auto it = spriteCache.find(path);
    if (it == spriteCache.end()) {
        Img img;
        if (keepAspect) {
            img.read(path, {CELL_SIZE, CELL_SIZE}, true);
        } else {
            img.read(path);
        }
        it = spriteCache.emplace(path, std::move(img)).first;
    }
    return it->second;
}

static int interpolatePixel(int fromCell, int toCell, double progress) {
    double fromPixel = fromCell * CELL_SIZE;
    double toPixel = toCell * CELL_SIZE;
    return static_cast<int>(fromPixel + (toPixel - fromPixel) * progress);
}

void Renderer::drawPiece(Img& boardImg, const PieceSnapshot& piece,
                          ImageView& imageView, int elapsedMs) {
    std::string code = pieceCode(piece.color, piece.kind);
    const AnimationConfigProvider& provider = providerFor(code);

    imageView.update(piece.id, provider, triggeredStateFor(piece.state), elapsedMs);

    std::string stateName = imageView.currentStateName(piece.id);
    int frame = imageView.currentFrame(piece.id);

    std::string path = std::string(ASSETS_DIR) + "/pieces_mine/" + code
        + "/states/" + stateName + "/sprites/" + std::to_string(frame + 1) + ".png";

    Img& pieceImg = cachedImage(path, true);

    int x, y;
    if (piece.motion.has_value()) {
        x = interpolatePixel(piece.motion->from.col, piece.motion->to.col, piece.motion->progress);
        y = interpolatePixel(piece.motion->from.row, piece.motion->to.row, piece.motion->progress);
    } else {
        x = piece.cell.col * CELL_SIZE;
        y = piece.cell.row * CELL_SIZE;
    }

    pieceImg.draw_on(boardImg, x, y);
}

int Renderer::render(const GameSnapshot& snapshot, ImageView& imageView, int elapsedMs,
                      const std::vector<Position>& highlightedCells,
                      const std::string& gameOverMessage,
                      const std::string& whiteName,
                      const std::string& blackName) {
    Img& boardTemplate = cachedImage(std::string(ASSETS_DIR) + "/board.png", false);

    const int PANEL_WIDTH = 220;
    cv::Mat canvas(boardTemplate.mat().rows, boardTemplate.mat().cols + PANEL_WIDTH,
                   boardTemplate.mat().type(), cv::Scalar(30, 30, 30));
    boardTemplate.mat().copyTo(canvas(cv::Rect(0, 0, boardTemplate.mat().cols, boardTemplate.mat().rows)));

    Img boardImg;
    boardImg.mat() = canvas;
    cv::Mat& mat = boardImg.mat();

    std::string whiteLabel = "White: " + whiteName + "  (" + std::to_string(snapshot.whiteScore) + ")";
    cv::putText(mat, whiteLabel, cv::Point(10, 26),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 5, cv::LINE_AA);
    cv::putText(mat, whiteLabel, cv::Point(10, 26),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);

    int baseline = 0;
    std::string blackLabel = "Black: " + blackName + "  (" + std::to_string(snapshot.blackScore) + ")";
    cv::Size blackSize = cv::getTextSize(blackLabel, cv::FONT_HERSHEY_SIMPLEX, 0.7, 2, &baseline);
    cv::Point blackOrigin(boardTemplate.mat().cols - blackSize.width - 10, 26);
    cv::putText(mat, blackLabel, blackOrigin,
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 5, cv::LINE_AA);
    cv::putText(mat, blackLabel, blackOrigin,
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);

    for (const PieceSnapshot& piece : snapshot.pieces) {
        drawPiece(boardImg, piece, imageView, elapsedMs);
    }

    for (const Position& cell : highlightedCells) {
        drawHighlight(mat, cell);
    }

    int panelX = boardTemplate.mat().cols + 10;
    cv::putText(mat, "Moves", cv::Point(panelX, 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

    int y = 45;
    int startIndex = snapshot.moveLog.size() > 15 ? static_cast<int>(snapshot.moveLog.size()) - 15 : 0;
    for (int i = startIndex; i < static_cast<int>(snapshot.moveLog.size()); i++) {
        const MoveLogEntry& entry = snapshot.moveLog[i];
        std::string line = std::to_string(entry.timeMs / 1000) + "s  " + entry.text;
        cv::putText(mat, line, cv::Point(panelX, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);
        y += 20;
    }

    if (!gameOverMessage.empty()) {
        cv::Mat overlay = mat.clone();
        cv::rectangle(overlay, cv::Rect(0, 0, mat.cols, mat.rows), cv::Scalar(0, 0, 0), cv::FILLED);
        cv::addWeighted(overlay, 0.55, mat, 0.45, 0, mat);

        cv::rectangle(mat, cv::Rect(30, 30, boardTemplate.mat().cols - 60, mat.rows - 60), cv::Scalar(0, 200, 255), 4);

        int titleFont = cv::FONT_HERSHEY_DUPLEX;
        double titleScale = 2.2;
        int titleThickness = 5;
        cv::Size titleSize = cv::getTextSize("GAME OVER", titleFont, titleScale, titleThickness, &baseline);
        cv::Point titleOrigin((boardTemplate.mat().cols - titleSize.width) / 2, mat.rows / 2 - 30);
        cv::putText(mat, "GAME OVER", titleOrigin, titleFont, titleScale, cv::Scalar(255, 255, 255), titleThickness, cv::LINE_AA);

        int subFont = cv::FONT_HERSHEY_DUPLEX;
        double subScale = 1.4;
        int subThickness = 3;
        cv::Size subSize = cv::getTextSize(gameOverMessage, subFont, subScale, subThickness, &baseline);
        cv::Point subOrigin((boardTemplate.mat().cols - subSize.width) / 2, mat.rows / 2 + 60);
        cv::putText(mat, gameOverMessage, subOrigin, subFont, subScale, cv::Scalar(0, 200, 255), subThickness, cv::LINE_AA);
    }

    return boardImg.showFrame();
}

