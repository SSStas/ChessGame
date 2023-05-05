#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "pieces.hpp"
#include "cellPos.hpp"


class Board {
    int size = 8;
    piecesList pieces;
    std::vector<piecesList::iterator> field;

    public:
        Board(int size);

        int getSize() {
            return size;
        }

        void clear();

        void setPiece(std::shared_ptr<Piece> newPiece);

        bool move(CellPos pos1, CellPos pos2);

        void getPossibleSteps(CellPos pos, std::list<CellPos> &steps);

        Color getPieceSide(CellPos pos);

        std::vector<int> getBoard();
};

bool classicStartPosition(Board& board);

class ChessGame {
    Board board;

    CellPos currentPos;
    Color playerTurn;
    std::list<CellPos> possibleSteps; 

    public:
        ChessGame();

        void clearChoosenPiece();

        bool makeStep(std::string str);

        bool isPieceChosen();

        bool choosePiece(std::string str);

        void showPossibleSteps();

        void show();
};
