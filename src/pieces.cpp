#include "pieces.hpp"


void Piece::getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::vector<CellPos> &steps) {
    int size = (int) sqrt(field.size());
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) { 
            if (field.at(i * size + j) == pieces.end() || (*field.at(i * size + j))->getSide() != pieceSide) {
                steps.push_back(CellPos(i + 1, j + 1)); 
            }
        }
    }
}
