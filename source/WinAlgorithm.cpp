//
// Created by admin on 4/17/2025.
//
#include "../headers/WinAlgorithm.h"

Action WinAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board) {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opponentPos;

    Direction currentDir = player->getDir();
    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    if(!player->isWaitingToReverse() && player->getWaitingToReverse())
         return Action::MoveBack;
    // Dodge incoming shells
    if (willBeHitIn(player->getPos().first,player->getPos().second,1,board) || willBeHitIn(player->getPos().first,player->getPos().second,2,board)) {
        if (canMoveFwd(board)) {
            return Action::MoveFwd;
        }
        if (canMoveBack(board)) {
            return Action::MoveBack;
        }

        //todo: for on all rotation options and return the first match
        player->rotate(Action::Rotate1_4Right);
        if(canMoveFwd(board)){
          player->setDir(currentDir);
          return Action::Rotate1_4Right;
        }
        return Action::Rotate1_8Right;
    }

    // Shoot if aligned and safe
    if (currentDir == desiredDir &&  !player->isWaitingToShoot()) {
//        if (!player->getPreparingToShoot()) {
//            player->setPreparingToShoot(true);
//        }

        return Action::Shoot;
    }
    if (currentDir != desiredDir) {
        return determineRotation(currentDir, desiredDir);
    }

    player->setPreparingToShoot(false);

    // Move forward if not aligned/blocked
    pair<int,int> next = player->nextStep(true, board.getHeight(), board.getWidth());
    if ((currentRow != targetRow || currentCol != targetCol) && isOccupierFree(next, board)) {
        return Action::MoveFwd;
    }
    // Default fallback action
    return Action::Rotate1_8Right;
}

WinAlgorithm::~WinAlgorithm() {
    player = nullptr;
}



