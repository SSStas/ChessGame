#include "chessGame.hpp"
#include <cmath>


Board::Board(int size) {
    this->size = size;
    field.assign(size * size, pieces.end());
}

void Board::clear() {
    field.assign(size * size, pieces.end());
    pieces.clear();
}

void Board::setPieces(std::shared_ptr<Piece> newPieces, CellPos pos) {
    if (!pos.isInSquareBoard(size)) {
        return;
    }

    pieces.push_back(newPieces);
    field.at(pos.getArrayPos(size)) = --pieces.end();
}

void Board::move(CellPos pos1, CellPos pos2) {
    if (field.at(pos1.getArrayPos(size)) != pieces.end() && !(pos1 == pos2)) {
        if (field.at(pos2.getArrayPos(size)) != pieces.end()) {
            pieces.erase(field.at(pos2.getArrayPos(size)));
        }
        field.at(pos2.getArrayPos(size)) = field.at(pos1.getArrayPos(size));
        field.at(pos1.getArrayPos(size)) = pieces.end();
    }
}

void Board::getPossibleSteps(CellPos pos, std::vector<CellPos> &steps) {
    (*field.at(pos.getArrayPos(size)))->getPossibleSteps(field, pieces, steps);
}

Color Board::getPieceSide(CellPos pos) {
    if (field.at(pos.getArrayPos(size)) != pieces.end()) {
        return (*field.at(pos.getArrayPos(size)))->getSide();
    }

    return NO_COLOR;
}

std::vector<int> Board::getBoard() {
    std::vector<int> fullBoard;
    for (int i = 0; i < size * size; ++i) {
        if (field.at(i) == pieces.end()) {
            fullBoard.push_back(0);
        } else {
            fullBoard.push_back((*field.at(i))->getKind() * (*field.at(i))->getSide());
        }
    }
    return fullBoard;
}

bool classicStartPosition(Board& board) {
    if (board.getSize() != 8) {
        return false;
    }

    board.clear();
    for (int i = 1; i <= 8; ++i) {
        board.setPieces(std::make_shared<Pawn>(WHITE), CellPos(i, 2));
        board.setPieces(std::make_shared<Pawn>(BLACK), CellPos(i, 7));
    }
    for (int i = 0; i < 2; ++i) {
        board.setPieces(std::make_shared<Rook>(WHITE), CellPos(1 + i * 7, 1));
        board.setPieces(std::make_shared<Rook>(BLACK), CellPos(1 + i * 7, 8));
        board.setPieces(std::make_shared<Knight>(WHITE), CellPos(2 + i * 5, 1));
        board.setPieces(std::make_shared<Knight>(BLACK), CellPos(2 + i * 5, 8));
        board.setPieces(std::make_shared<Bishop>(WHITE), CellPos(3 + i * 3, 1));
        board.setPieces(std::make_shared<Bishop>(BLACK), CellPos(3 + i * 3, 8));
    }
    board.setPieces(std::make_shared<Queen>(WHITE), CellPos(4, 1));
    board.setPieces(std::make_shared<Queen>(BLACK), CellPos(4, 8));
    board.setPieces(std::make_shared<King>(WHITE), CellPos(5, 1));
    board.setPieces(std::make_shared<King>(BLACK), CellPos(5, 8));

    return true;
} 

ChessGame::ChessGame(): board(8) {
    classicStartPosition(board);
    currentPos = CellPos();
    playerTurn = WHITE;
}

void ChessGame::clearChoosenPiece() {
    currentPos.setPosition(0, 0);
    possibleSteps.clear();
}

void ChessGame::makeStep(std::string str) {
    CellPos pos = CellPos(str);

    if (!isPieceChosen() || !pos.isInSquareBoard(board.getSize())) {
        return;
    }

    board.move(currentPos, pos);

    clearChoosenPiece();
    playerTurn = (playerTurn == WHITE) ? BLACK : ((playerTurn == BLACK) ? WHITE : NO_COLOR);
}

bool ChessGame::isPieceChosen() {
    return currentPos.isRealPos();
}

bool ChessGame::choosePiece(std::string str) {
    CellPos pos = CellPos(str);

    if (!pos.isInSquareBoard(board.getSize())) {
        clearChoosenPiece(); 
        return false;
    }

    if(board.getPieceSide(pos) == playerTurn) {
        if (currentPos != pos) {
            clearChoosenPiece(); 
            board.getPossibleSteps(pos, possibleSteps);
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

void ChessGame::show() {
    std::vector<int> fullBoard = board.getBoard();

    std::cout << "Board " << ((playerTurn == 1) ? "WHITE" : "BLACK") << std::endl;

    for (int i = 0; i < board.getSize(); ++i) {
        for (int j = 0; j < board.getSize(); ++j) {
            std::cout << std::setfill(' ') << std::setw(2) << fullBoard[j * board.getSize() + i] << " ";
        }
        std::cout << std::endl;
    }
}
