#include "Shell.h"
#include <utility> //idk why it doesnt pass through shells.h

void Shell::move(const GameBoard& board) {
        pair<int, int> fStep = nextStep(1, dir, board.getHeight(), board.getWidth());
        pair<int, int> sStep = nextStep(2, dir, board.getHeight(), board.getWidth());
        pos = {sStep.first, sStep.second};  // updated position after move
}
void Shell::goThroughWall(const pair<Cell*> cells) {
        for (const Cell* c : cells) {
                if (c.hasWall()) {
                        c.damageWall();
                }
        }
}

//need to implement
bool Shell::isValidMove(const GameBoard& board) {
        return true;  // or whatever you want
}
