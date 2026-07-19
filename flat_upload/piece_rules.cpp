// placeholder C++ file for piece rules (empty)
#include "piece_rules.hpp"

static void slide(const Board& board, const Piece& piece, int drow, int dcol, std::vector<Position>& out) {
    int row = piece.cell.row + drow;
    int col = piece.cell.col + dcol;

    while (board.isInBounds(Position(row, col))) {
        const Piece* occupant = board.pieceAt(Position(row, col));
        if (occupant == nullptr) {
            out.push_back(Position(row, col));
        } else {
            if (occupant->color != piece.color) {
                out.push_back(Position(row, col));
            }
            break;
        }
        row += drow;
        col += dcol;
    }
}

std::vector<Position> RookRule::legalDestinations(const Board& board, const Piece& piece) const {
    std::vector<Position> result;
    slide(board, piece, -1, 0, result);
    slide(board, piece, 1, 0, result);
    slide(board, piece, 0, -1, result);
    slide(board, piece, 0, 1, result);
    return result;
}

std::vector<Position> BishopRule::legalDestinations(const Board& board, const Piece& piece) const {
    std::vector<Position> result;
    slide(board, piece, -1, -1, result);
    slide(board, piece, -1, 1, result);
    slide(board, piece, 1, -1, result);
    slide(board, piece, 1, 1, result);
    return result;
}

std::vector<Position> QueenRule::legalDestinations(const Board& board, const Piece& piece) const {
    RookRule rook;
    BishopRule bishop;
    std::vector<Position> result = rook.legalDestinations(board, piece);
    std::vector<Position> diag = bishop.legalDestinations(board, piece);
    result.insert(result.end(), diag.begin(), diag.end());
    return result;
}

std::vector<Position> KnightRule::legalDestinations(const Board& board, const Piece& piece) const {
    static const int offsets[8][2] = {
        {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}
    };
    std::vector<Position> result;
    for (const auto& o : offsets) {
        Position dest(piece.cell.row + o[0], piece.cell.col + o[1]);
        if (!board.isInBounds(dest)) continue;
        const Piece* occupant = board.pieceAt(dest);
        if (occupant == nullptr || occupant->color != piece.color) {
            result.push_back(dest);
        }
    }
    return result;
}

std::vector<Position> KingRule::legalDestinations(const Board& board, const Piece& piece) const {
    std::vector<Position> result;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            Position dest(piece.cell.row + dr, piece.cell.col + dc);
            if (!board.isInBounds(dest)) continue;
            const Piece* occupant = board.pieceAt(dest);
            if (occupant == nullptr || occupant->color != piece.color) {
                result.push_back(dest);
            }
        }
    }
    return result;
}

const PieceRule& getPieceRule(Kind kind) {
    static RookRule rook;
    static BishopRule bishop;
    static QueenRule queen;
    static KnightRule knight;
    static KingRule king;

    switch (kind) {
        case Kind::Rook:   return rook;
        case Kind::Bishop: return bishop;
        case Kind::Queen:  return queen;
        case Kind::Knight: return knight;
        case Kind::King:   return king;
        default:           return king;  // Pawn not implemented this iteration
    }
}