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

Action BasicMoveAlgorithm::nextMove(OppData opp,const GameBoard& board){

    if(!player->isWaitingToReverse() && player->getWaitingToReverse())
        return Action::MoveBack;

    if (willBeHitIn(player->getPos().first,player->getPos().second,1,board)) {
        Action res=checkForEscape(board);
        if(res!=Action::None)
          return res;
        return Action::Rotate1_8Right;
    }

    if (shouldShootOpponent(opp.opponentPos)) {
        return Action::Shoot;
    }

    if (canMoveFwd(board)) {
        return Action::MoveFwd;
    }
    return Action::Rotate1_8Right;
}