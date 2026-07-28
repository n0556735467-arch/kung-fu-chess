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

void Renderer::drawPiece(Img& boardImg, const PieceSnapshot& piece,
                          ImageView& imageView, int elapsedMs) {
    std::string code = pieceCode(piece.color, piece.kind);
    const AnimationConfigProvider& provider = providerFor(code);

    imageView.update(piece.id, provider, triggeredStateFor(piece.state), elapsedMs);

    std::string stateName = imageView.currentStateName(piece.id);
    int frame = imageView.currentFrame(piece.id);

    std::string path = std::string(ASSETS_DIR) + "/pieces_mine/" + code
        + "/states/" + stateName + "/sprites/" + std::to_string(frame + 1) + ".png";

    Img pieceImg;
    pieceImg.read(path, {CELL_SIZE, CELL_SIZE});

    int x = piece.cell.col * CELL_SIZE;
    int y = piece.cell.row * CELL_SIZE;
    pieceImg.draw_on(boardImg, x, y);
}


static void drawHighlight(cv::Mat& boardMat, Position cell) {
    int x = cell.col * CELL_SIZE;
    int y = cell.row * CELL_SIZE;
    cv::rectangle(boardMat, cv::Rect(x, y, CELL_SIZE, CELL_SIZE), cv::Scalar(0, 220, 0), 2);
}

int Renderer::render(const GameSnapshot& snapshot, ImageView& imageView, int elapsedMs,
                      const std::vector<Position>& highlightedCells) {
    Img boardImg;
    boardImg.read(std::string(ASSETS_DIR) + "/board.png");

    for (const PieceSnapshot& piece : snapshot.pieces) {
        drawPiece(boardImg, piece, imageView, elapsedMs);
    }

    for (const Position& cell : highlightedCells) {
        drawHighlight(boardImg.mat(), cell);
    }

    return boardImg.showFrame();
}