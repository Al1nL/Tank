#include "../headers/Shell.h"

void Shell::move(const GameBoard& board) {
        pair<int, int> fStep = nextStep(1, dir, board.getHeight(), board.getWidth());
        pair<int, int> sStep = nextStep(2, dir, board.getHeight(), board.getWidth());
        pos = {sStep.first, sStep.second};  // updated position after move
}
void Shell::goThroughWall(pair<Cell*,Cell*> cells) {
        for (Cell* c : {cells.first, cells.second}){
                if (c->hasWall()) {
                        c->damageWall();
                }
        }
}

//need to implement
bool Shell::isValidMove(const GameBoard& board) {
        return true;  // or whatever you want
}
