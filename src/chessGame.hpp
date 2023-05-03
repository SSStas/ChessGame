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

        void setPieces(std::shared_ptr<Piece> newPieces, CellPos pos);

        void move(CellPos pos1, CellPos pos2);

        void getPossibleSteps(CellPos pos, std::vector<CellPos> &steps);

        Color getPieceSide(CellPos pos);

        std::vector<int> getBoard();
};

bool classicStartPosition(Board& board);

class ChessGame {
    Board board;

    CellPos currentPos;
    Color playerTurn;
    std::vector<CellPos> possibleSteps; 

    public:
        ChessGame();

        void clearChoosenPiece();

        void makeStep(std::string str);

        bool isPieceChosen();

        bool choosePiece(std::string str);

        void showPossibleSteps();

        void show();
};
