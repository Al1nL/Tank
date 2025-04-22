//
// Created by admin on 4/17/2025.
//
#include "../headers/WinAlgorithm.h"

Action WinAlgorithm::nextMove(pair<int,int> opponentPos,const GameBoard& board) {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opponentPos;

    Direction currentDir = player->getDir();
    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    // 1. Dodge incoming shells
    if (isInDanger(board)) {
        return Action::MoveFwd;
    }
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


//Action WinAlgorithm::strategicMove(pair<int, int> opponentPos, const vector<vector<CellType>>& map, const vector<Shell>& incomingShells) {
//    auto [currentRow, currentCol] = player->getPos();
//    auto [targetRow, targetCol] = opponentPos;
//
//    Direction currentDir = player->getDir();
//    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);
//
//    // 1. Dodge incoming shells
//    if (isInDanger()) {
//        return dodgeShell(currentRow, currentCol, map, incomingShells);
//    }
//
//    // 2. Avoid mines
//    if (isMineNearby(currentRow, currentCol, map)) {
//        return moveAwayFromMine(currentRow, currentCol, map);
//    }
//
//    // 3. Attempt to shoot the opponent if there's a direct view
//    if ( player->getRemainingShells() > 0) {
//        return Action::Shoot;
//        }
//
//    // 4. Align direction toward the opponent
//    if (currentDir != desiredDir) {
//        return determineRotation(currentDir, desiredDir);
//    }
//
//    // 5. Move closer to the opponent if there's no direct threat
//    if (currentRow != targetRow || currentCol != targetCol) {
//        return moveTowardOpponent(currentRow, currentCol, targetRow, targetCol, map);
//    }
//
//    // 6. Default fallback action
//    return Action::Rotate1_8Right;
//}

bool WinAlgorithm::isInDanger(const GameBoard& board) {
    pair<int,int> curr = player->getPos();
    pair<int,int> newP;
    for(int i =1; i<= 2; i++){
        for (const auto& pos : offsets) {
            newP = {curr.first + pos.first*i, curr.second + pos.second*i};
            if(board.at(newP).hasShell()){
              return true;
            }
        }
    }
    return false;
}

//Action dodgeShell(int row, int col, const vector<vector<CellType>>& map, const vector<Shell>& incomingShells) {
//    // Calculate safe positions nearby and move to the safest one
//    vector<pair<int, int>> safePositions = getSafePositions(row, col, map, incomingShells);
//    if (!safePositions.empty()) {
//        return moveToClosestSafePosition(row, col, safePositions);
//    }
//    return Action::Rotate1_8Right; // Default dodge rotation if no move is possible
//}


//Action moveTowardOpponent(int row, int col, int targetRow, int targetCol, const vector<vector<CellType>>& map) {
//    vector<pair<int, int>> path = findPathToTarget(row, col, targetRow, targetCol, map);
//    if (!path.empty()) {
//        pair<int, int> nextStep = path.front();
//        return calculateMoveAction(row, col, nextStep.first, nextStep.second);
//    }
//    return Action::Rotate1_8Right; // Default fallback
//}

vector<pair<int, int>> WinAlgorithm::getSafePositions(const GameBoard& board) {
    vector<pair<int, int>> safePositions;
    const auto [row, col] = player->getPos();
    // Directions to check: up, down, left, right
    vector<pair<int, int>> directions = offsets;
    for (const auto& [dr, dc] : directions) {
        int newRow = row + dr, newCol = col + dc;
        if (board.at({newRow,newCol}).getOccupierType() == OccupierType::None && !board.at({newRow,newCol}).hasShell() ) {
            newRow += dr;
            newCol += dc;
            if (!board.at({newRow,newCol}).hasShell()) {
                safePositions.emplace_back(newRow, newCol);
            }
        }
    }
    return safePositions;
}

//Action moveToClosestSafePosition(int row, int col, const vector<pair<int, int>>& safePositions) {
//    if (safePositions.empty()) {
//        return Action::Rotate1_8Right; // Fallback
//    }
//
//    // Assume the first position is the best for simplicity
//    auto [safeRow, safeCol] = safePositions.front();
//    return calculateMoveAction(row, col, safeRow, safeCol);
//}
//
//vector<pair<int, int>> findSafeCells(int row, int col, const vector<vector<CellType>>& map) {
//    vector<pair<int, int>> safeCells;
//
//    // Directions to check: up, down, left, right
//    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
//    for (const auto& [dr, dc] : directions) {
//        int newRow = row + dr, newCol = col + dc;
//        if (isValidCell(newRow, newCol, map) && map[newRow][newCol] != CellType::Mine) {
//            safeCells.emplace_back(newRow, newCol);
//        }
//    }
//    return safeCells;
//}
