#include "chessGame.hpp"


void Piece::getPossibleSteps(std::list<std::shared_ptr<Piece>>::iterator* field, std::list<std::shared_ptr<Piece>> &pieces, int size, std::vector<int> &steps) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) { 
            if (field[i * size + j] == pieces.end() || (*field[i * size + j])->getSide() != pieceSide) {
                steps.push_back(i);
                steps.push_back(j);
            }
        }
    }
}

Board::Board(int size) {
    this->size = size;
    field = new std::list<std::shared_ptr<Piece>>::iterator[size * size];
    for (int i = 0; i < size * size; ++i) {
        field[i] = pieces.end();
    }
}

Board::~Board() {
    delete[] field;
}

void Board::clear() {
    pieces.clear();
    for (int i = 0; i < size * size; ++i) {
        field[i] = pieces.end();
    }
}

void Board::setPieces(std::shared_ptr<Piece> newPieces, int letterPos, int numberPos) {
    if (!(0 <= letterPos && letterPos < size) || !(0 <= numberPos && numberPos < size)) {
        return;
    }

    pieces.push_back(newPieces);
    field[letterPos * size + numberPos] = --pieces.end();
}

void Board::move(int letterPos1, int numberPos1, int letterPos2, int numberPos2) {
    if (field[letterPos1 * size + numberPos1] != pieces.end() && !(letterPos1 == letterPos2 && letterPos2 == numberPos2)) {
        if (field[letterPos2 * size + numberPos2] != pieces.end()) {
            pieces.erase(field[letterPos2 * size + numberPos2]);
        }
        field[letterPos2 * size + numberPos2] = field[letterPos1 * size + numberPos1];
        field[letterPos1 * size + numberPos1] = pieces.end();
    } 
}

void Board::getPossibleSteps(int letterPos, int numberPos, std::vector<int> &steps) {
    (*field[letterPos * size + numberPos])->getPossibleSteps(field, pieces, size, steps);
}

Color Board::getPieceSide(int letterPos, int numberPos) {
    if (field[letterPos * size + numberPos] != pieces.end()) {
        return (*field[letterPos * size + numberPos])->getSide();
    }

    return NO_COLOR;
}

std::vector<int> Board::getBoard() {
    std::vector<int> fullBoard;
    for (int i = 0; i < size * size; ++i) {
        if (field[i] == pieces.end()) {
            fullBoard.push_back(0);
        } else {
            std::shared_ptr<Piece> piecesParams = *field[i];
            fullBoard.push_back(piecesParams->getKind() * piecesParams->getSide());
        }
    }
    return fullBoard;
}

bool classicStartPosition(Board& board) {
    if (board.getSize() != 8) {
        return false;
    }

    board.clear();
    for (int i = 0; i < 8; ++i) {
        board.setPieces(std::make_shared<Pawn>(WHITE), i, 1);
        board.setPieces(std::make_shared<Pawn>(BLACK), i, 6);
    }
    for (int i = 0; i < 2; ++i) {
        board.setPieces(std::make_shared<Rook>(WHITE), i * 7, 0);
        board.setPieces(std::make_shared<Rook>(BLACK), i * 7, 7);
        board.setPieces(std::make_shared<Knight>(WHITE), 1 + i * 5, 0);
        board.setPieces(std::make_shared<Knight>(BLACK), 1 + i * 5, 7);
        board.setPieces(std::make_shared<Bishop>(WHITE), 2 + i * 3, 0);
        board.setPieces(std::make_shared<Bishop>(BLACK), 2 + i * 3, 7);
    }
    board.setPieces(std::make_shared<Queen>(WHITE), 3, 0);
    board.setPieces(std::make_shared<Queen>(BLACK), 3, 7);
    board.setPieces(std::make_shared<King>(WHITE), 4, 0);
    board.setPieces(std::make_shared<King>(BLACK), 4, 7);

    return true;
} 

ChessGame::ChessGame(): board(8) {
    classicStartPosition(board);
    letterPosNow = -1;
    numberPosNow = -1;
    playerTurn = WHITE;
}

void ChessGame::clearChoosenPiece() {
    letterPosNow = -1;
    numberPosNow = -1;
    possibleSteps.clear();
}

void ChessGame::makeStep(std::string pos) {
    int letterPos = int(pos[0] - 'a'), numberPos = int(pos[1] - '1');

    if (!isPieceChosen() || !(0 <= letterPos && letterPos < board.getSize()) || !(0 <= numberPos && numberPos < board.getSize())) {
        return;
    }

    board.move(letterPosNow, numberPosNow, letterPos, numberPos);

    clearChoosenPiece();
    playerTurn = (playerTurn == WHITE) ? BLACK : ((playerTurn == BLACK) ? WHITE : NO_COLOR);
}

bool ChessGame::isPieceChosen() {
    return (letterPosNow != -1 && numberPosNow != -1);
}

bool ChessGame::choosePiece(std::string pos) {
    int letterPos = int(pos[0] - 'a'), numberPos = int(pos[1] - '1');

    if (!(0 <= letterPos && letterPos < board.getSize()) || !(0 <= numberPos && numberPos < board.getSize())) {
        clearChoosenPiece(); 
        return false;
    }

    if(board.getPieceSide(letterPos, numberPos) == playerTurn) {
        if (letterPos != letterPosNow && numberPos != numberPosNow) {
            clearChoosenPiece(); 
            board.getPossibleSteps(letterPos, numberPos, possibleSteps);
            letterPosNow = letterPos;
            numberPosNow = numberPos;
        }
        return true;
    }

    return false;
}

void ChessGame::showPossibleSteps() {
    std::cout << "PossibleSteps " << possibleSteps.size() / 2 << std::endl;

    for (long unsigned int i = 0; i < possibleSteps.size(); i += 2) {
        std::cout << char('a' + possibleSteps.at(i)) << 1 + possibleSteps.at(i + 1) << " ";
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