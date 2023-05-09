#pragma once

#include <vector>
#include <list>
#include <memory>
#include <cmath>
#include "cellPos.hpp"


enum Color { NO_COLOR = 0, WHITE = 1, BLACK = -1 };

enum PieceKind { NONE = 0, KING = 1, QUEEN, BISHOP, ROOK, KNIGHT, PAWN };

enum LogStatus { RECORDING_ON = 0, RECORDING_OFF };

enum PieceMoveStatus { REMOVAL = 0, ADDITION, REMOVAL_PRIORITY, ADDING_PRIORITY };

class Piece;

using piecesList = std::list<std::shared_ptr<Piece>>;
using pieceMoving = std::pair<std::shared_ptr<Piece>, PieceMoveStatus>;
using step = std::pair<CellPos, CellPos>;

// The position of the pieces on the chessboard

class Board {
    std::list<piecesList::iterator> toDeletePieces;

    public:
        const int size;
        piecesList pieces;
        std::vector<piecesList::iterator> field;

        Board(int size);

        void setPiece(std::shared_ptr<Piece> newPiece);

        std::shared_ptr<Piece> &getPiecePtr(CellPos pos);
        std::shared_ptr<Piece> &getPiecePtr(int index);

        bool isCellEmpty(CellPos pos);
        bool isCellEmpty(int index);

        void deletePiece(CellPos pos);

        // safety way to delete piece elements
        void putInDeleteList(CellPos pos);
        void deletePiecesFromList();
        void clearDeleteList();

        void replacePiece(CellPos fromPos, CellPos toPos);

        void clear();
};

// Record of moves in a chess game

struct SavedMoving_t {
    step mainPieceStep;
    Color mainPieceSide;
    PieceKind mainPieceKind;

    std::list<pieceMoving> piecesMove;

    SavedMoving_t(step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind) {
        this->mainPieceStep = mainPieceStep;
        this->mainPieceSide = mainPieceSide;
        this->mainPieceKind = mainPieceKind;
    }
};

class MoveLog {
    LogStatus logStatus;

    std::list<SavedMoving_t> log;
    std::list<SavedMoving_t>::iterator lastState;

    public:
        MoveLog(LogStatus logStatus): logStatus(logStatus) {
            log.assign(1, SavedMoving_t({CellPos(), CellPos()}, NO_COLOR, NONE));
            lastState = log.begin();
        }

        bool isLogRecordOn() { return logStatus == RECORDING_ON; }
        void turnRecordingOn();
        void turnRecordingOff();

        void createNewRecord(step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind);
        void addNewMoveRecord(std::shared_ptr<Piece> &piecePtr, PieceMoveStatus status);

        bool goNextRecord(Board &board);
        bool goPreviousRecord(Board &board);

        step getLastMoveStep();
        Color getLastMoveColor();
        PieceKind getLastMoveKind();
};

// Data and structure of movement of chess pieces

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

        virtual void move(Board &board, MoveLog &moveLog, CellPos newPos);
        virtual void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        virtual bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos);

        virtual std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Piece>(*this);
        }
};

class King: public Piece {
    public:
        King(CellPos position, Color color) : Piece(position, color) {
            this->kind = KING;
        }

        void move(Board &board, MoveLog &moveLog, CellPos newPos);
        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos);

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<King>(*this);
        }
};

class Queen: public Piece {
    protected:
        void getLinearPosSteps(std::pair<int, int> dir[4], Board &board, std::list<CellPos> &steps);
        bool isLinearPosSteps(std::pair<int, int> dir[4], Board &board, CellPos otherPos);

    public:
        Queen(CellPos position, Color color) : Piece(position, color) {
            this->kind = QUEEN;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Queen>(*this);
        }
};

class Bishop: public Queen {
    public:
        Bishop(CellPos position, Color color) : Queen(position, color) {
            this->kind = BISHOP;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Bishop>(*this);
        }
};

class Rook: public Queen { 
    public:
        Rook(CellPos position, Color color) : Queen(position, color) {
            this->kind = ROOK;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Rook>(*this);
        }
};

class Knight: public Piece {
    public:
        Knight(CellPos position, Color color) : Piece(position, color) {
            this->kind = KNIGHT;
        }

        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 

        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Knight>(*this);
        }
};

class Pawn: public Piece {
    enum Direct { UP, DOWN };

    Direct direction;

    int getDirEnPassent(MoveLog &moveLog, CellPos currentPos);

    public:
        Pawn(CellPos position, Color color, bool isFirstStep) : Piece(position, color) {
            this->kind = PAWN;
            this->isFirstStep = isFirstStep;
            this->direction = (this->pieceSide == WHITE) ? UP : DOWN;
        }

        void move(Board &board, MoveLog &moveLog, CellPos newPos);
        void getPossibleSteps(Board &board, MoveLog &moveLog, std::list<CellPos> &steps);
        bool isPossibleStep(Board &board, MoveLog &moveLog, CellPos otherPos); 
        
        std::shared_ptr<Piece> copySharedPtr() {
            return std::make_shared<Pawn>(*this);
        }
};
