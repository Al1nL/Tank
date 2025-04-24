//
// Created by admin on 4/18/2025.
//

#include "../headers/Algorithm.h"

Direction Algorithm::calculateDirection(int currRow, int currCol, int targetRow, int targetCol) {
    if (currRow < targetRow && currCol == targetCol) return Direction::D;
    if (currRow > targetRow && currCol == targetCol) return Direction::U;
    if (currRow == targetRow && currCol < targetCol) return Direction::R;
    if (currRow == targetRow && currCol > targetCol) return Direction::L;
    if (currRow < targetRow && currCol < targetCol) return Direction::DR;
    if (currRow < targetRow && currCol > targetCol) return Direction::DL;
    if (currRow > targetRow && currCol < targetCol) return Direction::UR;
    if (currRow > targetRow && currCol > targetCol) return Direction::UL;

    return player->getDir(); // Default to current direction if no match
}

Action Algorithm::determineRotation(Direction currentDir, Direction desiredDir) {
    int rotationSteps = (static_cast<int>(desiredDir) - static_cast<int>(currentDir) + 8) % 8;
    if (rotationSteps == 1 || rotationSteps == 2) return Action::Rotate1_8Right;
    if (rotationSteps == 6 || rotationSteps == 7) return Action::Rotate1_8Left;
    if (rotationSteps == 3) return Action::Rotate1_4Right;
    if (rotationSteps == 5) return Action::Rotate1_4Left;

    // If no rotation required
    return Action::Rotate1_8Right; // Arbitrary fallback
}

bool Algorithm::isOccupierFree(pair<int,int> pos, const GameBoard& board)  {
    OccupierType type = board.at(pos).getOccupierType();
    return type == OccupierType::None;
}

bool Algorithm::shouldShootOpponent(const pair<int,int>& opponentPos)  {
    return player->getRemainingShells() > 0 &&
           isAlignedWithOpponent(opponentPos) &&
           !player->isWaitingToShoot();
}

bool Algorithm::canMoveFwd(const GameBoard& board)  {
    if (!player->isValidMove(board, Action::MoveFwd)) return false;
    auto [r, c] = player->nextStep(true, board.getHeight(), board.getWidth());
    return isOccupierFree({r, c}, board);
}

bool Algorithm::canMoveBack(const GameBoard& board)  {
    if (!player->isValidMove(board, Action::MoveBack)) return false;
    pair<int,int> next = player->nextStep(false, board.getHeight(), board.getWidth());
    return canSafelyBack(next.first, next.second, board) && isOccupierFree(next, board);
}

bool Algorithm::canSafelyBack(int backR, int backC, const GameBoard& board)  {
    auto [r,c]=player->getPos();
    // current position and backward cell are safe
    if (!player->getMovedBackwardLast()) {
        if (willBeHitIn(backR, backC, 4, board)) {//willBeHitIn(r, c, 1, board) || willBeHitIn(r, c, 2, board) ||
            return false; // Danger in current or backward position
        }
    }
    else {
        // If just moved back, only check immediate danger in next cell
        if (willBeHitIn(backR, backC, 2, board)) {
            return false;
        }
    }
    return true; // Safe to move backward
}


bool Algorithm::willBeHitIn(int row, int col, int t, const GameBoard& board) {
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


bool Algorithm::isInDanger(const GameBoard& board) {
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


bool Algorithm::isAlignedWithOpponent(pair<int, int> opponentPos) {
    auto [r,c]=player->getPos();
    return (r == opponentPos.first || c == opponentPos.second );  // Aligned if same row or column
}
