//
// Created by admin on 4/17/2025.
//
#include "../headers/WinAlgorithm.h"

vector<pair<int, int>> WinAlgorithm::getNeighbors(int x, int y) {
    return {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
}

Action WinAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board) {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opponentPos;

    Direction currentDir = player->getDir();
    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    // Rotate if not aligned
    if (currentDir != desiredDir) {
        return determineRotation(currentDir, desiredDir);
    }

    if ( player->getRemainingShells() > 0) {
        return Action::Shoot;
    }

    // Move forward if aligned
    if (currentRow != targetRow || currentCol != targetCol && (board.at(player->nextStep(true,board.getHeight()))).getOccupierType() != OccupierType::Mine){
        return Action::MoveFwd;
    }


    // Default fallback action
    return Action::Rotate1_8Right;
}

WinAlgorithm::~WinAlgorithm() {
    player = nullptr;
}

//bool WinAlgorithm::hasDirectView(pair<int, int> opponentPos,const GameBoard& board){
//      pair<int,int> offset = offsets[player->getDir()];
//        auto [currentRow, currentCol] = player->getPos();
//    while (currentRow != opponentPos.first && currentCol != opponentPos.second) {
//
//        if (isObstacle(currentRow, currentCol)) {
//            return false; // Path is blocked
//        }
//
//        // Advance in the current direction
//        std::tie(currentRow, currentCol) = advanceInDirection(currentRow, currentCol, dir);
//    }
//  }