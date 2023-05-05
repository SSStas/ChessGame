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

void Board::setPiece(std::shared_ptr<Piece> newPiece) {
    CellPos pos = newPiece->getPos();

    if (!pos.isInSquareBoard(size)) {
        return;
    }

    pieces.push_back(newPiece);
    field.at(pos.getArrayPos(size)) = --pieces.end();
}

bool Board::move(CellPos pos1, CellPos pos2) {
    if (!pos1.isInSquareBoard(size) || !pos2.isInSquareBoard(size)) {
        return false;
    }

    piecesList::iterator pieceIt = field.at(pos1.getArrayPos(size));
    if (pieceIt != pieces.end() && pos1 != pos2 && (*pieceIt)->isPossibleStep(field, pieces, pos2)) {
        (*pieceIt)->move(field, pieces, pos2);
        return true;
    }

    return false;
}

void Board::getPossibleSteps(CellPos pos, std::list<CellPos> &steps) {
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
        board.setPiece(std::make_shared<Pawn>(CellPos(i, 2), WHITE, true));
        board.setPiece(std::make_shared<Pawn>(CellPos(i, 7), BLACK, true));
    }
    for (int i = 0; i < 2; ++i) {
        board.setPiece(std::make_shared<Rook>(CellPos(1 + i * 7, 1), WHITE));
        board.setPiece(std::make_shared<Rook>(CellPos(1 + i * 7, 8), BLACK));
        board.setPiece(std::make_shared<Knight>(CellPos(2 + i * 5, 1), WHITE));
        board.setPiece(std::make_shared<Knight>(CellPos(2 + i * 5, 8), BLACK));
        board.setPiece(std::make_shared<Bishop>(CellPos(3 + i * 3, 1), WHITE));
        board.setPiece(std::make_shared<Bishop>(CellPos(3 + i * 3, 8), BLACK));
    }
    board.setPiece(std::make_shared<Queen>(CellPos(4, 1), WHITE));
    board.setPiece(std::make_shared<Queen>(CellPos(4, 8), BLACK));
    board.setPiece(std::make_shared<King>(CellPos(5, 1), WHITE));
    board.setPiece(std::make_shared<King>(CellPos(5, 8), BLACK));

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

bool ChessGame::makeStep(std::string str) {
    CellPos pos = CellPos(str);

    if (!board.move(currentPos, pos)) {
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
