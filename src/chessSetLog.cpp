#include "chessSet.hpp"


void MoveLog::turnRecordingOn() {
    logStatus = RECORDING_ON;
}

void MoveLog::turnRecordingOff() {
    log.assign(1, SavedMoving_t(lastState->mainPieceStep, lastState->mainPieceSide, lastState->mainPieceKind));
    lastState = log.begin();

    logStatus = RECORDING_OFF;
}

void MoveLog::createNewRecord(step mainPieceStep, Color mainPieceSide, PieceKind mainPieceKind) {
    if (logStatus == RECORDING_OFF) {
        log.assign(1, SavedMoving_t(mainPieceStep, mainPieceSide, mainPieceKind));
        lastState = log.begin();
    } else {
        auto it = log.begin();
        while (it != lastState) {
            it = log.erase(it);
        }

        log.push_front(SavedMoving_t(mainPieceStep, mainPieceSide, mainPieceKind));
        lastState = log.begin();
    }
    std::cout << log.size() << "\n"; 
}

void MoveLog::addNewMoveRecord(std::shared_ptr<Piece> &piecePtr, PieceMoveStatus status) {
    if (logStatus == RECORDING_OFF) {
        return;
    }

    lastState->piecesMove.push_front({piecePtr->copySharedPtr(), status});
}

bool MoveLog::goNextRecord(Board &board) {
    if (lastState == log.begin() || logStatus == RECORDING_OFF) {
        return false;
    }

    std::cout << log.size() << "\n"; 

    --lastState;

    auto it = lastState->piecesMove.end();

    while (it != lastState->piecesMove.begin()) {
        --it;
        CellPos piecePos = it->first->getPos();

        if (!piecePos.isRealPos()) {
            continue;
        }

        board.deletePiece(piecePos);

        if (it->second == ADDITION || it->second == ADDING_PRIORITY) {
            board.setPiece(it->first->copySharedPtr());
        } 
        
        if (it->second == ADDING_PRIORITY) {
            // TODO RuleController checking
        }

        if (it->second == REMOVAL_PRIORITY) {
            // TODO RuleController checking
        }
    }

    return true;
}

bool MoveLog::goPreviousRecord(Board &board) {
    if (lastState == log.end() || std::next(lastState) == log.end() || logStatus == RECORDING_OFF) {
        return false;
    }

    std::cout << log.size() << "\n"; 

    for (auto it = lastState->piecesMove.begin(); it != lastState->piecesMove.end(); ++it) {
        CellPos piecePos = it->first->getPos();
        
        if (!piecePos.isRealPos()) {
            continue;
        }

        board.deletePiece(piecePos);

        if (it->second == REMOVAL || it->second == REMOVAL_PRIORITY) {
            board.setPiece(it->first->copySharedPtr());
        } 
        
        if (it->second == REMOVAL_PRIORITY) {
            // TODO RuleController checking
        }

        if (it->second == ADDING_PRIORITY) {
            // TODO RuleController checking
        }
    }
    ++lastState;

    return true;
}

step MoveLog::getLastMoveStep() {
    return lastState->mainPieceStep;
}

Color MoveLog::getLastMoveColor() {
    return lastState->mainPieceSide;
}

PieceKind MoveLog::getLastMoveKind() {
    return lastState->mainPieceKind;
}
