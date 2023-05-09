#include "chessGame.hpp"
#include <cmath>


ChessGame::ChessGame(): chess(), currentPos() { 
    playerTurn = WHITE;
}

bool ChessGame::isPossibleStep(CellPos pos) {
    for (auto posIt = possibleSteps.begin(); posIt != possibleSteps.end(); ++posIt) {
        if (*posIt == pos) {
            return true;
        }
    }

    return false;
}

void ChessGame::clearChoosenPiece() {
    currentPos.setPosition(0, 0);
    possibleSteps.clear();
}

bool ChessGame::makeStep(std::string str) {
    CellPos pos = CellPos(str);

    if (!chess.move(currentPos, pos)) {
        clearChoosenPiece();
        return false;
    }

    clearChoosenPiece();
    playerTurn = (playerTurn == WHITE) ? BLACK : ((playerTurn == BLACK) ? WHITE : NO_COLOR);
    return true;
}

bool ChessGame::isPieceChosen() {
    return currentPos.isRealPos();
}

bool ChessGame::choosePiece(std::string str) {
    CellPos pos = CellPos(str);

    if (!pos.isInSquareBoard(chess.getSize())) {
        clearChoosenPiece(); 
        return false;
    }

    if (!isPossibleStep(pos) && chess.getPieceSide(pos) == playerTurn) {
        if (currentPos != pos) {
            clearChoosenPiece(); 
            chess.getPossibleSteps(pos, possibleSteps);
            currentPos = pos;
        }
        return true;
    }

    return false;
}

void ChessGame::showPossibleSteps() {
    std::cout << "PossibleSteps " << possibleSteps.size() << std::endl;

    for (auto it = possibleSteps.begin(); it != possibleSteps.end(); ++it) {
        std::cout << char('a' + it->getLetter() - 1) << it->getNumber() << " ";
    }
    std::cout << std::endl;
}

LogStatus ChessGame::changeRecordingStatus() {
    LogStatus newStatus = chess.isRecordingOn() ? RECORDING_OFF : RECORDING_ON;
    chess.setRecordingStatus(newStatus);
    return newStatus;
}

void ChessGame::nextMove() {
    clearChoosenPiece();
    if (chess.nextMove())
        playerTurn = (playerTurn == WHITE) ? BLACK : ((playerTurn == BLACK) ? WHITE : NO_COLOR);
}

void ChessGame::previousMove() {
    clearChoosenPiece();
    if (chess.previousMove())
        playerTurn = (playerTurn == WHITE) ? BLACK : ((playerTurn == BLACK) ? WHITE : NO_COLOR);
}

void ChessGame::show() {
    std::vector<int> fullBoard = chess.getBoard();

    std::cout << "Board " << ((playerTurn == 1) ? "WHITE" : "BLACK") << std::endl;

    for (int i = 0; i < chess.getSize(); ++i) {
        for (int j = 0; j < chess.getSize(); ++j) {
            std::cout << std::setfill(' ') << std::setw(2) << fullBoard[j * chess.getSize() + i] << " ";
        }
        std::cout << std::endl;
    }
}
