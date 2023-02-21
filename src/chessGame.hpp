#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <memory>
#include <string>


enum Color { NO_COLOR = 0, WHITE = 1, BLACK = -1 };
enum PieceKind { NONE = 0, KING = 1, QUEEN, BISHOP, KNIGHT, ROOK, PAWN };

class Piece {
    protected:
        Color pieceSide;

    public:
        Piece(Color color) {
            pieceSide = color;
        }

        virtual PieceKind getKind() = 0;
        Color getSide() { return this->pieceSide; }

        void getPossibleSteps(std::list<std::shared_ptr<Piece>>::iterator* field, std::list<std::shared_ptr<Piece>> &pieces, int size, std::vector<int> &steps);
};

class King: public Piece {
    PieceKind kind;

    public:
        King(Color color) : Piece(color) {
            this->kind = KING;
        }

        PieceKind getKind() { return this->kind; }
};

class Queen: public Piece {
    PieceKind kind;

    public:
        Queen(Color color) : Piece(color) {
            this->kind = QUEEN;
        }

        PieceKind getKind() { return this->kind; }
};

class Bishop: public Piece {
    PieceKind kind;

    public:
        Bishop(Color color) : Piece(color) {
            this->kind = BISHOP;
        }

        PieceKind getKind() { return this->kind; }
};

class Knight: public Piece {
    PieceKind kind;

    public:
        Knight(Color color) : Piece(color) {
            this->kind = KNIGHT;
        }

        PieceKind getKind() { return this->kind; }
};

class Rook: public Piece {
    PieceKind kind;

    public:
        Rook(Color color) : Piece(color) {
            this->kind = ROOK;
        }

        PieceKind getKind() { return this->kind; }
};

class Pawn: public Piece {
    PieceKind kind;

    public:
        Pawn(Color color) : Piece(color) {
            this->kind = PAWN;
        }

        PieceKind getKind() { return this->kind; }
};

class Board {
    int size = 8;
    std::list<std::shared_ptr<Piece>> pieces;
    std::list<std::shared_ptr<Piece>>::iterator* field;

    public:
        Board(int size);

        ~Board();

        int getSize() {
            return size;
        }

        void clear();

        void setPieces(std::shared_ptr<Piece> newPieces, int letterPos, int numberPos);

        void move(int letterPos1, int numberPos1, int letterPos2, int numberPos2);

        void getPossibleSteps(int letterPos, int numberPos, std::vector<int> &steps);

        Color getPieceSide(int letterPos, int numberPos);

        std::vector<int> getBoard();
};

bool classicStartPosition(Board& board);

class ChessGame {
    Board board;

    int letterPosNow;
    int numberPosNow;
    Color playerTurn;
    std::vector<int> possibleSteps; 

    public:
        ChessGame();

        void clearChoosenPiece();

        void makeStep(std::string pos);

        bool isPieceChosen();

        bool choosePiece(std::string pos);

        void showPossibleSteps();

        void show();
};
 