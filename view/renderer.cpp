#include "renderer.hpp"
#include "../model/constants.hpp"

static char colorChar(Color color) {
    return color == Color::White ? 'w' : 'b';
}

static char kindChar(Kind kind) {
    switch (kind) {
        case Kind::King:   return 'K';
        case Kind::Queen:  return 'Q';
        case Kind::Rook:   return 'R';
        case Kind::Bishop: return 'B';
        case Kind::Knight: return 'N';
        case Kind::Pawn:   return 'P';
    }
    return '?';
}

static std::string stateFolder(PieceState state) {
    switch (state) {
        case PieceState::Moving:   return "move";
        case PieceState::Airborne: return "jump";
        default:                   return "idle";
    }
}

std::string Renderer::spritePath(const Piece& piece) const {
    std::string code;
    code += colorChar(piece.color);
    code += kindChar(piece.kind);

    return R"(C:\Users\User\Desktop\kung-fu-chess\assets\pieces_mine\)"
        + code + "\\states\\" + stateFolder(piece.state) + "\\sprites\\1.png";
}

void Renderer::drawPiece(Img& boardImg, const Piece& piece) {
    Img pieceImg;
    pieceImg.read(spritePath(piece), {CELL_SIZE, CELL_SIZE});

    int x = piece.cell.col * CELL_SIZE;
    int y = piece.cell.row * CELL_SIZE;

    pieceImg.draw_on(boardImg, x, y);
}

void Renderer::render(const Board& board) {
    Img boardImg;
    boardImg.read(R"(C:\Users\User\Desktop\kung-fu-chess\assets\board.png)");

    for (const Piece& piece : board.getPieces()) {
        drawPiece(boardImg, piece);
    }

    boardImg.show();
}