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
        if (player->isValidMove(board, Action::MoveFwd)) return Action::MoveFwd;
        auto [backR, backC] = player->nextStep(false, board.getHeight(), board.getWidth());
        if (player->isValidMove(board, Action::MoveBack) && canSafelyBack(backR, backC, board)) {
            return Action::MoveBack;
        }
        return Action::Rotate1_8Right;
    }
//    // Rotate if not aligned
//    if (currentDir != desiredDir) {
//        return determineRotation(currentDir, desiredDir);
//    }
    // Shoot if aligned and safe
    if (player->getRemainingShells() > 0 &&isAlignedWithOpponent(opponentPos)&&!player->isWaitingToShoot()) {
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
    OccupierType nextOcc =board.at(player->nextStep(true,board.getHeight(),board.getWidth())).getOccupierType();
    if ((currentRow != targetRow || currentCol != targetCol) && (nextOcc != OccupierType::Mine&&nextOcc != OccupierType::Wall)){
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
            newP = {board.positiveMod(newP.first, board.getHeight()), board.positiveMod(newP.second, board.getWidth())};
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


bool WinAlgorithm::willBeHitIn(int row, int col, int t, const GameBoard& board) {
    auto shells = board.getAllFiredShells();  // Get all fired shells
    for (Shell* s : shells) {
        auto [sr, sc] = s->getPos();
        Direction sd = s->getDir();
        auto [dr, dc] = offsets[static_cast<int>(sd)];

        // Project forward `t` steps:
        int pr = sr + dr * t;
        int pc = sc + dc * t;

        // Wraparound or bounds-check if needed:
        pr = wrap(pr, board.getHeight());
        pc = wrap(pc, board.getWidth());

        if (pr == row && pc == col) {
            return true;  // A shell will hit the cell by that time
        }
    }
    return false;
}

bool WinAlgorithm::isWalkable(int r, int c, const GameBoard& board) {
    return board.at({r, c}).isWalkable();  // Check if the cell is walkable
}

bool WinAlgorithm::isAlignedWithOpponent(pair<int, int> opponentPos) {
    auto [r,c]=player->getPos();
  return (r == opponentPos.first || c == opponentPos.second );  // Aligned if same row or column
}

bool WinAlgorithm::canSafelyBack(int backR, int backC, const GameBoard& board) {
    auto [r,c]=player->getPos();
    // current position and backward cell are safe
    if (!player->getMovedBackwardLast()) {
        if (willBeHitIn(r, c, 1, board) || willBeHitIn(r, c, 2, board) || willBeHitIn(backR, backC, 3, board)) {
            return false; // Danger in current or backward position
        }
    }
    else {
        // If just moved back, only check immediate danger in next cell
        if (willBeHitIn(backR, backC, 1, board)) {
            return false;
        }
    }
    return true; // Safe to move backward
}
