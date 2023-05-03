#pragma once

#include <vector>
#include <list>
#include <memory>
#include <cmath>
#include "cellPos.hpp"


enum Color { NO_COLOR = 0, WHITE = 1, BLACK = -1 };

enum PieceKind { NONE = 0, KING = 1, QUEEN, BISHOP, KNIGHT, ROOK, PAWN };

class Piece;

using piecesList = std::list<std::shared_ptr<Piece>>;

class Piece {
    protected:
        Color pieceSide;
        PieceKind kind;

    public:
        Piece(Color color) {
            pieceSide = color;
            kind = NONE;
        }

        PieceKind getKind() { return this->kind; }
        Color getSide() { return this->pieceSide; }

        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::vector<CellPos> &steps);
};

class King: public Piece {
    public:
        King(Color color) : Piece(color) {
            this->kind = KING;
        }
};

class Queen: public Piece {
    public:
        Queen(Color color) : Piece(color) {
            this->kind = QUEEN;
        }
};

class Bishop: public Piece {
    public:
        Bishop(Color color) : Piece(color) {
            this->kind = BISHOP;
        }
};

class Knight: public Piece {
    public:
        Knight(Color color) : Piece(color) {
            this->kind = KNIGHT;
        }
};

class Rook: public Piece {
    public:
        Rook(Color color) : Piece(color) {
            this->kind = ROOK;
        }
};

class Pawn: public Piece {
    public:
        Pawn(Color color) : Piece(color) {
            this->kind = PAWN;
        }
};
