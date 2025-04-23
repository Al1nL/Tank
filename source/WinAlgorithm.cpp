//
// Created by admin on 4/17/2025.
//
#include "../headers/WinAlgorithm.h"

Action WinAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board) {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opponentPos;

    Direction currentDir = player->getDir();
    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    // Dodge incoming shells
    if (isInDanger(board)) {
        if (canMoveFwd(board)) {
            return Action::MoveFwd;
        }
        if (canMoveBack(board)) {
            return Action::MoveBack;
        }
        return Action::Rotate1_8Right;
    }

    // Shoot if aligned and safe
    if (shouldShootOpponent(opponentPos)) {
        if (!player->getPreparingToShoot()) {
            player->setPreparingToShoot(true);
        }

        if (currentDir != desiredDir) {
            return determineRotation(currentDir, desiredDir);
        }

        return Action::Shoot;
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



