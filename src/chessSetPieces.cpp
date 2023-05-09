#include "chessSet.hpp"


void Piece::move(Board &board, MoveLog &moveLog, CellPos newPos) {
    moveLog.createNewRecord({position, newPos}, pieceSide, kind);

    if (!board.isCellEmpty(newPos) && board.getPiecePtr(position) != board.getPiecePtr(newPos)) {
        moveLog.addNewMoveRecord(board.getPiecePtr(newPos), REMOVAL);
        board.deletePiece(newPos);
    }

    moveLog.addNewMoveRecord(board.getPiecePtr(position), REMOVAL);
    board.replacePiece(position, newPos);

    position = newPos;
    isFirstStep = false;
    moveLog.addNewMoveRecord(board.getPiecePtr(position), ADDITION);
}

void Piece::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) { 
            if (board.isCellEmpty(i * board.size + j) || board.getPiecePtr(i * board.size + j)->getSide() != pieceSide) {
                steps.push_back(CellPos(i + 1, j + 1)); 
            }
        }
    }
}

bool Piece::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) { 
            if (board.isCellEmpty(i * board.size + j) || board.getPiecePtr(i * board.size + j)->getSide() != pieceSide) {
                return true;
            }
        }
    }

    return false;
}

void King::move(Board &board, MoveLog &moveLog, CellPos newPos) {
    CellPos rookPos = CellPos();

    // castling checking
    if (abs(newPos.getLetter() - position.getLetter()) > 1) {
        int dis = 1;
        int dir = (newPos.getLetter() > position.getLetter()) ? 1 : -1;

        moveLog.createNewRecord({position, newPos}, pieceSide, kind);

        do {
            rookPos.setPosition(position.getLetter() + dir * dis, position.getNumber());
            ++dis;
        } while(board.isCellEmpty(rookPos));

        // rook moving
        CellPos newRookPos = CellPos(position.getLetter() + dir, position.getNumber());
        moveLog.addNewMoveRecord(board.getPiecePtr(rookPos), REMOVAL);
        board.replacePiece(rookPos, newRookPos);
        board.getPiecePtr(newRookPos)->setIsFirstStep(false);
        board.getPiecePtr(newRookPos)->setPos(newRookPos);
        moveLog.addNewMoveRecord(board.getPiecePtr(newRookPos), ADDITION);

        // king moving
        moveLog.addNewMoveRecord(board.getPiecePtr(position), REMOVAL);
        board.replacePiece(position, newPos);
        isFirstStep = false;
        position = newPos;  
        moveLog.addNewMoveRecord(board.getPiecePtr(newPos), ADDITION);
    } else {
        Piece::move(board, moveLog, newPos);
    }
}

void King::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    CellPos newPos = CellPos();
    
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) { 
            newPos.setPosition(position.getLetter() + i, position.getNumber() + j);
            
            if (position != newPos && newPos.isInSquareBoard(board.size) && 
            (board.isCellEmpty(newPos) || board.getPiecePtr(newPos)->getSide() != pieceSide)) {
                steps.push_back(newPos); 
            }
        }
    }

    if (!isFirstStep) {
        return;
    }

    // castling
    for (int i = -1; i <= 1; i += 2) {
        int dis = 1;
        newPos.setPosition(position.getLetter() + i * dis, position.getNumber());

        while(newPos.isInSquareBoard(board.size)) {
            if (!board.isCellEmpty(newPos)) { 
                if (dis >= 2 && board.getPiecePtr(newPos)->getSide() == pieceSide && board.getPiecePtr(newPos)->getKind() == ROOK && 
                board.getPiecePtr(newPos)->getIsFirstStep()) {
                    steps.push_front(CellPos(position.getLetter() + 2 * i, position.getNumber()));
                }
                break;
            }
            
            ++dis;
            newPos.setPosition(position.getLetter() + i * dis, position.getNumber());
        }
    }
}

bool King::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    CellPos newPos = CellPos();
    
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) { 
            newPos.setPosition(position.getLetter() + i, position.getNumber() + j);

            if (position != newPos && newPos.isInSquareBoard(board.size) && newPos == otherPos && 
            (board.isCellEmpty(newPos) || board.getPiecePtr(newPos)->getSide() != pieceSide)) { 
                return true;
            }
        }
    }

    if (!isFirstStep) {
        return false;
    }

    // castling
    for (int i = -1; i <= 1; i += 2) {
        int dis = 1;
        newPos.setPosition(position.getLetter() + i * dis, position.getNumber());

        while(newPos.isInSquareBoard(board.size)) {
            if (!board.isCellEmpty(newPos)) { 
                if (dis >= 2 && board.getPiecePtr(newPos)->getSide() == pieceSide && board.getPiecePtr(newPos)->getKind() == ROOK && 
                board.getPiecePtr(newPos)->getIsFirstStep()) {
                    newPos.setPosition(position.getLetter() + 2 * i, position.getNumber());
                    if (newPos == otherPos) {
                        return true;
                    }
                }
                break;
            }

            ++dis;
            newPos.setPosition(position.getLetter() + i * dis, position.getNumber());
        }
    }

    return false;
}

void Queen::getLinearPosSteps(std::pair<int, int> dir[4], Board &board, std::list<CellPos> &steps) {
    CellPos newPos = CellPos();

    for (int i = 0; i < 4; ++i) {
        int dis = 1;
        newPos.setPosition(position.getLetter() + dir[i].first * dis, position.getNumber() + dir[i].second * dis);
        
        while(newPos.isInSquareBoard(board.size)) {
            if (!board.isCellEmpty(newPos)) {
                if (board.getPiecePtr(newPos)->getSide() != pieceSide) {
                    steps.push_back(newPos);
                }
                break;
            } else {
                steps.push_back(newPos);
            }

            ++dis;
            newPos.setPosition(position.getLetter() + dir[i].first * dis, position.getNumber() + dir[i].second * dis);
        }
    }
}

bool Queen::isLinearPosSteps(std::pair<int, int> dir[4], Board &board, CellPos otherPos) {
    CellPos newPos = CellPos();

    for (int i = 0; i < 4; ++i) {
        int dis = 1;
        newPos.setPosition(position.getLetter() + dir[i].first * dis, position.getNumber() + dir[i].second * dis);
        
        while(newPos.isInSquareBoard(board.size)) {
            if (!board.isCellEmpty(newPos)) {
                if (board.getPiecePtr(newPos)->getSide() != pieceSide && newPos == otherPos) {
                    return true;
                } 
                break;
            } else if (newPos == otherPos) {
                return true;
            }

            ++dis;
            newPos.setPosition(position.getLetter() + dir[i].first * dis, position.getNumber() + dir[i].second * dis);
        }
    }

    return false;
}

void Queen::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    getLinearPosSteps(dir, board, steps);

    std::pair<int, int> dir2[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    getLinearPosSteps(dir2, board, steps);
}

bool Queen::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::pair<int, int> dir2[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    return (isLinearPosSteps(dir, board, otherPos) || isLinearPosSteps(dir2, board, otherPos));
}

void Bishop::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    std::pair<int, int> dir[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    getLinearPosSteps(dir, board, steps);
}

bool Bishop::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    std::pair<int, int> dir[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    return isLinearPosSteps(dir, board, otherPos);
}

void Rook::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    getLinearPosSteps(dir, board, steps);
}

bool Rook::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    return isLinearPosSteps(dir, board, otherPos);
}

void Knight::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    std::pair<int, int> posIndex[8] = {{2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}};
    CellPos newPos = CellPos();

    for (int i = 0; i < 8; ++i) {
        newPos.setPosition(position.getLetter() + posIndex[i].first, position.getNumber() + posIndex[i].second);
        if (newPos.isInSquareBoard(board.size) && (board.isCellEmpty(newPos) || board.getPiecePtr(newPos)->getSide() != pieceSide)) {
            steps.push_back(newPos);
        }
    }
}

bool Knight::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    std::pair<int, int> posIndex[8] = {{2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}};
    CellPos newPos = CellPos();

    for (int i = 0; i < 8; ++i) {
        newPos.setPosition(position.getLetter() + posIndex[i].first, position.getNumber() + posIndex[i].second);
        if (newPos.isInSquareBoard(board.size) && newPos == otherPos && 
        (board.isCellEmpty(newPos) || board.getPiecePtr(newPos)->getSide() != pieceSide)) {
            return true;
        }
    }

    return false;
} 

int Pawn::getDirEnPassent(MoveLog &moveLog, CellPos currentPos) {
    if (moveLog.getLastMoveColor() != pieceSide && moveLog.getLastMoveKind() == PAWN && 
    abs(moveLog.getLastMoveStep().first.getNumber() - moveLog.getLastMoveStep().second.getNumber()) > 1 &&
    moveLog.getLastMoveStep().second.getNumber() == currentPos.getNumber()) {
        if (currentPos.getLetter() - moveLog.getLastMoveStep().second.getLetter() == 1) {
            return -1;
        } else if (moveLog.getLastMoveStep().second.getLetter() - currentPos.getLetter() == 1) {
            return 1;
        }
    }

    return 0;
}

void Pawn::move(Board &board, MoveLog &moveLog, CellPos newPos) {
    int dir = (direction == UP) ? 1 : -1;
    int dirEnPassent = getDirEnPassent(moveLog, position);
    CellPos possiblePos = CellPos(position.getLetter() + dirEnPassent, position.getNumber() + dir);

    if (possiblePos.isInSquareBoard(board.size) && possiblePos == newPos && possiblePos != position && dirEnPassent != 0) {
        CellPos enemyPos = moveLog.getLastMoveStep().second;
        moveLog.createNewRecord({position, newPos}, pieceSide, kind);

        if (!board.isCellEmpty(enemyPos) && board.getPiecePtr(position) != board.getPiecePtr(enemyPos)) {
            moveLog.addNewMoveRecord(board.getPiecePtr(enemyPos), REMOVAL);
            board.deletePiece(enemyPos);
        }

        moveLog.addNewMoveRecord(board.getPiecePtr(position), REMOVAL);
        board.replacePiece(position, newPos);

        position = newPos;
        isFirstStep = false;
        moveLog.addNewMoveRecord(board.getPiecePtr(position), ADDITION);
    } else {
        Piece::move(board, moveLog, newPos);
    }
}

void Pawn::getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps) {
    int dir = (direction == UP) ? 1 : -1;
    CellPos newPos = CellPos(position.getLetter(), position.getNumber() + dir);

    int dirEnPassent = getDirEnPassent(moveLog, position);

    if(newPos.isInSquareBoard(board.size) && board.isCellEmpty(newPos)) {
        steps.push_back(newPos);
        newPos.setPosition(position.getLetter(), position.getNumber() + 2 * dir);
        if (isFirstStep && newPos.isInSquareBoard(board.size) && board.isCellEmpty(newPos)) {
            steps.push_back(newPos);
        }
    }

    for (int i = -1; i <= 1; i += 2) {
        newPos.setPosition(position.getLetter() + i, position.getNumber() + dir);
        if (newPos.isInSquareBoard(board.size) && 
        ((!board.isCellEmpty(newPos) && board.getPiecePtr(newPos)->getSide() != pieceSide) || dirEnPassent == i)) {
            steps.push_back(newPos); 
        }
    }
}

bool Pawn::isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos) {
    int dir = (direction == UP) ? 1 : -1;
    CellPos newPos = CellPos(position.getLetter(), position.getNumber() + dir);

    int dirEnPassent = getDirEnPassent(moveLog, position);

    if(newPos.isInSquareBoard(board.size) && board.isCellEmpty(newPos)) {
        if (newPos == otherPos) {
            return true;
        }
        newPos.setPosition(position.getLetter(), position.getNumber() + 2 * dir);
        if (isFirstStep && newPos.isInSquareBoard(board.size) && board.isCellEmpty(newPos) && newPos == otherPos) {
            return true;
        }
    }

    for (int i = -1; i <= 1; i += 2) {
        newPos.setPosition(position.getLetter() + i, position.getNumber() + dir);
        if (newPos.isInSquareBoard(board.size) && newPos == otherPos && 
        ((!board.isCellEmpty(newPos) && board.getPiecePtr(newPos)->getSide() != pieceSide) || dirEnPassent == i)) {
            return true;
        }
    }

    return false;
}
