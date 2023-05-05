#include "pieces.hpp"


void Piece::move(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos newPos) {
    int size = (int) sqrt(field.size());

    if (field.at(newPos.getArrayPos(size)) != pieces.end() && 
    *field.at(position.getArrayPos(size)) != *field.at(newPos.getArrayPos(size))) {
        pieces.erase(field.at(newPos.getArrayPos(size)));
    }

    field.at(newPos.getArrayPos(size)) = field.at(position.getArrayPos(size));
    field.at(position.getArrayPos(size)) = pieces.end();

    position = newPos;
    isFirstStep = false;
}

void Piece::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    int size = (int) sqrt(field.size());

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) { 
            if (field.at(i * size + j) == pieces.end() || (*field.at(i * size + j))->getSide() != pieceSide) {
                steps.push_back(CellPos(i + 1, j + 1)); 
            }
        }
    }
}

bool Piece::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    int size = (int) sqrt(field.size());

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) { 
            if (field.at(i * size + j) == pieces.end() || (*field.at(i * size + j))->getSide() != pieceSide) {
                return true;
            }
        }
    }

    return false;
}

void King::move(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos newPos) {
    int size = (int) sqrt(field.size());
    CellPos rookPos = CellPos();

    // castling checking
    if (abs(newPos.getLetter() - position.getLetter()) > 1) {
        int dis = 1;
        int dir = (newPos.getLetter() > position.getLetter()) ? 1 : -1;

        do {
            rookPos.setPosition(position.getLetter() + dir * dis, position.getNumber());
            ++dis;
        } while(field.at(rookPos.getArrayPos(size)) == pieces.end());

        // rook moving
        CellPos newRookPos = CellPos(position.getLetter() + dir, position.getNumber());
        field.at(newRookPos.getArrayPos(size)) = field.at(rookPos.getArrayPos(size));
        field.at(rookPos.getArrayPos(size)) = pieces.end();
        (*field.at(newRookPos.getArrayPos(size)))->setIsFirstStep(false);
        (*field.at(newRookPos.getArrayPos(size)))->setPos(newRookPos);

        // king moving
        field.at(newPos.getArrayPos(size)) = field.at(position.getArrayPos(size));
        field.at(position.getArrayPos(size)) = pieces.end();
        isFirstStep = false;
        position = newPos;  
    } else {
        Piece::move(field, pieces, newPos);
    }
}

void King::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    int size = (int) sqrt(field.size());
    CellPos newPos = CellPos();
    
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) { 
            newPos.setPosition(position.getLetter() + i, position.getNumber() + j);
            
            if (position != newPos && newPos.isInSquareBoard(size) && 
            (field.at(newPos.getArrayPos(size)) == pieces.end() || (*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide)) {
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

        while(newPos.isInSquareBoard(size)) {
            if (field.at(newPos.getArrayPos(size)) != pieces.end()) {
                if (dis >= 2 && (*field.at(newPos.getArrayPos(size)))->getSide() == pieceSide && 
                (*field.at(newPos.getArrayPos(size)))->getKind() == ROOK && (*field.at(newPos.getArrayPos(size)))->getIsFirstStep()) {
                    steps.push_front(CellPos(position.getLetter() + 2 * i, position.getNumber()));
                }
                break;
            }
            
            ++dis;
            newPos.setPosition(position.getLetter() + i * dis, position.getNumber());
        }
    }
}

bool King::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    int size = (int) sqrt(field.size());
    CellPos newPos = CellPos();
    
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) { 
            newPos.setPosition(position.getLetter() + i, position.getNumber() + j);

            if (position != newPos && newPos.isInSquareBoard(size) && newPos == otherPos && 
            (field.at(newPos.getArrayPos(size)) == pieces.end() || (*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide)) {
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

        while(newPos.isInSquareBoard(size)) {
            if (field.at(newPos.getArrayPos(size)) != pieces.end()) {
                if (dis >= 2 && (*field.at(newPos.getArrayPos(size)))->getSide() == pieceSide && 
                (*field.at(newPos.getArrayPos(size)))->getKind() == ROOK && (*field.at(newPos.getArrayPos(size)))->getIsFirstStep()) {
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

void Queen::getLinearPosSteps(std::pair<int, int> dir[4], std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    int size = (int) sqrt(field.size());
    CellPos newPos = CellPos();

    for (int i = 0; i < 4; ++i) {
        int dis = 1;
        newPos.setPosition(position.getLetter() + dir[i].first * dis, position.getNumber() + dir[i].second * dis);
        
        while(newPos.isInSquareBoard(size)) {
            if (field.at(newPos.getArrayPos(size)) != pieces.end()) {
                if ((*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide) {
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

bool Queen::isLinearPosSteps(std::pair<int, int> dir[4], std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    int size = (int) sqrt(field.size());
    CellPos newPos = CellPos();

    for (int i = 0; i < 4; ++i) {
        int dis = 1;
        newPos.setPosition(position.getLetter() + dir[i].first * dis, position.getNumber() + dir[i].second * dis);
        
        while(newPos.isInSquareBoard(size)) {
            if (field.at(newPos.getArrayPos(size)) != pieces.end()) {
                if ((*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide && newPos == otherPos) {
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

void Queen::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    getLinearPosSteps(dir, field, pieces, steps);

    std::pair<int, int> dir2[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    getLinearPosSteps(dir2, field, pieces, steps);
}

bool Queen::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::pair<int, int> dir2[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    return (isLinearPosSteps(dir, field, pieces, otherPos) || isLinearPosSteps(dir2, field, pieces, otherPos));
}

void Bishop::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    std::pair<int, int> dir[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    getLinearPosSteps(dir, field, pieces, steps);
}

bool Bishop::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    std::pair<int, int> dir[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    return isLinearPosSteps(dir, field, pieces, otherPos);
}

void Rook::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    getLinearPosSteps(dir, field, pieces, steps);
}

bool Rook::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    std::pair<int, int> dir[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    return isLinearPosSteps(dir, field, pieces, otherPos);
}

void Knight::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    std::pair<int, int> posIndex[8] = {{2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}};
    int size = (int) sqrt(field.size());
    CellPos newPos = CellPos();

    for (int i = 0; i < 8; ++i) {
        newPos.setPosition(position.getLetter() + posIndex[i].first, position.getNumber() + posIndex[i].second);
        if (newPos.isInSquareBoard(size) && 
        (field.at(newPos.getArrayPos(size)) == pieces.end() || (*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide)) {
            steps.push_back(newPos);
        }
    }
}

bool Knight::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    std::pair<int, int> posIndex[8] = {{2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}};
    int size = (int) sqrt(field.size());
    CellPos newPos = CellPos();

    for (int i = 0; i < 8; ++i) {
        newPos.setPosition(position.getLetter() + posIndex[i].first, position.getNumber() + posIndex[i].second);
        if (newPos.isInSquareBoard(size) && newPos == otherPos && 
        (field.at(newPos.getArrayPos(size)) == pieces.end() || (*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide)) {
            return true;
        }
    }

    return false;
} 

void Pawn::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps) {
    int size = (int) sqrt(field.size());
    int dir = (direction == UP) ? 1 : -1;
    CellPos newPos = CellPos(position.getLetter(), position.getNumber() + dir);

    if(newPos.isInSquareBoard(size) && field.at(newPos.getArrayPos(size)) == pieces.end()) {
        steps.push_back(newPos);
        newPos.setPosition(position.getLetter(), position.getNumber() + 2 * dir);
        if (isFirstStep && newPos.isInSquareBoard(size) && field.at(newPos.getArrayPos(size)) == pieces.end()) {
            steps.push_back(newPos);
        }
    }

    for (int i = -1; i <= 1; i += 2) {
        newPos.setPosition(position.getLetter() + i, position.getNumber() + dir);
        if (newPos.isInSquareBoard(size) && (field.at(newPos.getArrayPos(size)) != pieces.end() &&
        (*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide)) {
            steps.push_back(newPos); 
        }
    }
}

bool Pawn::isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos) {
    int size = (int) sqrt(field.size());
    int dir = (direction == UP) ? 1 : -1;
    CellPos newPos = CellPos(position.getLetter(), position.getNumber() + dir);

    if(newPos.isInSquareBoard(size) && field.at(newPos.getArrayPos(size)) == pieces.end()) {
        if (newPos == otherPos) {
            return true;
        }
        newPos.setPosition(position.getLetter(), position.getNumber() + 2 * dir);
        if (isFirstStep && newPos.isInSquareBoard(size) && field.at(newPos.getArrayPos(size)) == pieces.end() && newPos == otherPos) {
            return true;
        }
    }

    for (int i = -1; i <= 1; i += 2) {
        newPos.setPosition(position.getLetter() + i, position.getNumber() + dir);
        if (newPos.isInSquareBoard(size) && (field.at(newPos.getArrayPos(size)) != pieces.end() && newPos == otherPos &&
        (*field.at(newPos.getArrayPos(size)))->getSide() != pieceSide)) {
            return true;
        }
    }

    return false;
}
