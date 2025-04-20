//
// Created by admin on 4/20/2025.
//
#include "../headers/BasicMoveAlgorithm.h"

Action BasicMoveAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board){
    auto [currentRow, currentCol] = player->getPos();
    Direction currentDir = player->getDir();
    auto [r, c] = offsets[currentDir];
    if(board.at({currentRow-r,currentCol-c}).getOccupierType() != OccupierType::Wall){
      return Action::MoveFwd;
    }
    return Action::Rotate1_4Right;
}