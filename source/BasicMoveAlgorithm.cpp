//
// Created by admin on 4/20/2025.
//
#include "../headers/BasicMoveAlgorithm.h"
//
//Action BasicMoveAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board){
//    auto [currentRow, currentCol] = player->getPos();
//    Direction currentDir = player->getDir();
//    auto [r, c] = offsets[dir];
//
//
//        if(board.at({currentRow-r,currentCol-c}).getOccupierType() != OccupierType::Wall){
//          return Action::MoveFwd;
//        }
//
//    return Action::Rotate1_4Right;
//}

/**
* If in danger and backward is possible: start backward move with 2-step delay.

  If already waiting for backward: wait until cooldown hits 0, then move back.

  If a forward move becomes available while waiting for back: cancel the delayed back.

  Otherwise: default behavior (move forward unless there's a wall, else rotate).
*/
Action BasicMoveAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board){
    pair<int,int> fwdPos = nextStep(true,board.getHeight(),board.getWidth());

    if(board.at(fwdPos).getOccupierType() != OccupierType::Wall){
        return Action::MoveFwd;
    }
    return Action::Rotate1_4Right;
}