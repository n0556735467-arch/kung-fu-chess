#include "snapshot_codec.hpp"
#include <sstream>

namespace {

std::string colorToString(Color c) { return c == Color::White ? "White" : "Black"; }
Color colorFromString(const std::string& s) { return s == "White" ? Color::White : Color::Black; }

std::string kindToString(Kind k) {
    switch (k) {
        case Kind::King:   return "King";
        case Kind::Queen:  return "Queen";
        case Kind::Rook:   return "Rook";
        case Kind::Bishop: return "Bishop";
        case Kind::Knight: return "Knight";
        case Kind::Pawn:   return "Pawn";
    }
    return "Pawn";
}
Kind kindFromString(const std::string& s) {
    if (s == "King") return Kind::King;
    if (s == "Queen") return Kind::Queen;
    if (s == "Rook") return Kind::Rook;
    if (s == "Bishop") return Kind::Bishop;
    if (s == "Knight") return Kind::Knight;
    return Kind::Pawn;
}

std::string stateToString(PieceState s) {
    switch (s) {
        case PieceState::Idle:     return "Idle";
        case PieceState::Moving:   return "Moving";
        case PieceState::Airborne: return "Airborne";
        case PieceState::Captured: return "Captured";
    }
    return "Idle";
}
PieceState stateFromString(const std::string& s) {
    if (s == "Moving") return PieceState::Moving;
    if (s == "Airborne") return PieceState::Airborne;
    if (s == "Captured") return PieceState::Captured;
    return PieceState::Idle;
}

}

std::string encodeSnapshot(const GameSnapshot& snapshot) {
    std::ostringstream out;
    out << "ROWS " << snapshot.rows << "\n";
    out << "COLS " << snapshot.cols << "\n";
    out << "GAMEOVER " << (snapshot.gameOver ? 1 : 0) << "\n";
    out << "WINNER " << (snapshot.winner.has_value() ? colorToString(snapshot.winner.value()) : "None") << "\n";
    out << "WHITESCORE " << snapshot.whiteScore << "\n";
    out << "BLACKSCORE " << snapshot.blackScore << "\n";

    for (const PieceSnapshot& p : snapshot.pieces) {
        out << "PIECE " << p.id << " " << colorToString(p.color) << " "
            << kindToString(p.kind) << " " << stateToString(p.state) << " "
            << p.cell.row << " " << p.cell.col;
        if (p.motion.has_value()) {
            out << " MOTION " << p.motion->from.row << " " << p.motion->from.col
                << " " << p.motion->to.row << " " << p.motion->to.col
                << " " << p.motion->progress;
        }
        out << "\n";
    }

    for (const MoveLogEntry& entry : snapshot.moveLog) {
        out << "MOVELOG " << entry.timeMs << " " << entry.text << "\n";
    }

    out << "END\n";
    return out.str();
}

GameSnapshot decodeSnapshot(const std::string& text) {
    GameSnapshot snap;
    std::istringstream in(text);
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream lineStream(line);
        std::string tag;
        lineStream >> tag;

        if (tag == "ROWS") {
            lineStream >> snap.rows;
        } else if (tag == "COLS") {
            lineStream >> snap.cols;
        } else if (tag == "GAMEOVER") {
            int v; lineStream >> v; snap.gameOver = (v != 0);
        } else if (tag == "WHITESCORE") {
            lineStream >> snap.whiteScore;
        } else if (tag == "BLACKSCORE") {
            lineStream >> snap.blackScore;
        } else if (tag == "PIECE") {
    int id;
    std::string colorStr, kindStr, stateStr;
    int cellRow, cellCol;
    lineStream >> id >> colorStr >> kindStr >> stateStr >> cellRow >> cellCol;

    PieceSnapshot p{
        id,
        colorFromString(colorStr),
        kindFromString(kindStr),
        stateFromString(stateStr),
        Position(cellRow, cellCol),
        std::nullopt
    };

    std::string maybeMotion;
    if (lineStream >> maybeMotion && maybeMotion == "MOTION") {
        int fromRow, fromCol, toRow, toCol;
        double progress;
        lineStream >> fromRow >> fromCol >> toRow >> toCol >> progress;
        p.motion = MotionSnapshot{Position(fromRow, fromCol), Position(toRow, toCol), progress};
    }
    snap.pieces.push_back(p);
}else if (tag == "MOVELOG") {
            MoveLogEntry entry;
            lineStream >> entry.timeMs;
            std::getline(lineStream, entry.text);
            if (!entry.text.empty() && entry.text[0] == ' ') {
                entry.text = entry.text.substr(1);
            }
            snap.moveLog.push_back(entry);
        } else if (tag == "WINNER") {
    std::string winnerStr;
    lineStream >> winnerStr;
    if (winnerStr != "None") {
        snap.winner = colorFromString(winnerStr);
    }

        } else if (tag == "END") {
            break;
        }
    }

    return snap;
}