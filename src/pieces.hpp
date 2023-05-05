#pragma once

#include <vector>
#include <list>
#include <memory>
#include <cmath>
#include "cellPos.hpp"


enum Color { NO_COLOR = 0, WHITE = 1, BLACK = -1 };

enum PieceKind { NONE = 0, KING = 1, QUEEN, BISHOP, ROOK, KNIGHT, PAWN };

class Piece;

using piecesList = std::list<std::shared_ptr<Piece>>;

class Piece {
    protected:
        CellPos position;
        Color pieceSide;
        PieceKind kind;
        bool isFirstStep;

    public:
        Piece(CellPos position, Color color) {
            this->position = position;
            pieceSide = color;
            kind = NONE;
            isFirstStep = true;
        }

        PieceKind getKind() { return this->kind; }
        Color getSide() { return this->pieceSide; }
        CellPos getPos() { return this->position; }
        bool getIsFirstStep() { return this->isFirstStep; }

        void setIsFirstStep(bool val) { this->isFirstStep = val; }
        void setPos(CellPos position) { this->position = position; }

        virtual void move(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos newPos);
        virtual void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        virtual bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos);
};

class King: public Piece {
    public:
        King(CellPos position, Color color) : Piece(position, color) {
            this->kind = KING;
        }

        void move(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos newPos);
        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos);
};

class Queen: public Piece {
    protected:
        void getLinearPosSteps(std::pair<int, int> dir[4], std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isLinearPosSteps(std::pair<int, int> dir[4], std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos);

    public:
        Queen(CellPos position, Color color) : Piece(position, color) {
            this->kind = QUEEN;
        }

        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos); 
};

class Bishop: public Queen {
    public:
        Bishop(CellPos position, Color color) : Queen(position, color) {
            this->kind = BISHOP;
        }

        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos); 
};

class Rook: public Queen { 
    public:
        Rook(CellPos position, Color color) : Queen(position, color) {
            this->kind = ROOK;
        }

        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos); 
};

class Knight: public Piece {
    public:
        Knight(CellPos position, Color color) : Piece(position, color) {
            this->kind = KNIGHT;
        }

        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos); 
};

class Pawn: public Piece {
    enum Direct { UP, DOWN };

    Direct direction;

    public:
        Pawn(CellPos position, Color color, bool isFirstStep) : Piece(position, color) {
            this->kind = PAWN;
            this->isFirstStep = isFirstStep;
            this->direction = (this->pieceSide == WHITE) ? UP : DOWN;
        }

        void getPossibleSteps(std::vector<piecesList::iterator> &field, piecesList &pieces, std::list<CellPos> &steps);
        bool isPossibleStep(std::vector<piecesList::iterator> &field, piecesList &pieces, CellPos otherPos); 
};
