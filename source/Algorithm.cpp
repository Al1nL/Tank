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
