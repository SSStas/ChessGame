#include "chessGame.hpp"
#include <string>
#include <map>
#include <windows.h>

int main() {
    ChessGame game = ChessGame();
    bool isGameContinue = true;
    std::string cmd = "", pos = "";

    while (isGameContinue) {
        std::cin >> cmd;
        
        if (cmd == "exit") {
            isGameContinue = false;
        } else {
            if (!game.choosePiece(cmd) && game.isPieceChosen()) {
                game.makeStep(cmd);
                game.show();
            } else {
                game.showPossibleSteps();
            }
        }
    }

    return 0;
}