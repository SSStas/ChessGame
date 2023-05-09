#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "chessSet.hpp"
#include "cellPos.hpp"


class RulesController {
    public:
        RulesController() {}    
};

class Chess {
    Board board; 
    MoveLog moveLog;

    public:
        Chess(int size = 8, LogStatus recordingStatus = RECORDING_ON): board(size), moveLog(recordingStatus) {
            classicStartPosition();
        }

        int getSize() {
            return board.size;
        }

        void getPossibleSteps(CellPos pos, std::list<CellPos> &steps);
        Color getPieceSide(CellPos pos);
        

        bool classicStartPosition(); 

        bool move(CellPos pos1, CellPos pos2);

        bool isRecordingOn();
        void setRecordingStatus(LogStatus recordingStatus);
        bool nextMove();
        bool previousMove();


        std::vector<int> getBoard();
};
