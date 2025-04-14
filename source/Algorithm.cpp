#include "../headers/Algorithm.h"


vector<pair<int, int>> WinAlgorithm::getNeighbors(int x, int y) {
    return {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
}

Action WinAlgorithm::nextMove() {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opponent->getPos();

    Movable::Direction currentDir = player->getDir();
	Movable::Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    // Rotate if not aligned
    if (currentDir != desiredDir) {
        return determineRotation(currentDir, desiredDir);
    }

    // Move forward if aligned
    if (currentRow != targetRow || currentCol != targetCol) {
        return Action::MoveFwd;
    }

    // Shoot if aligned with the target
    if (player->getRemainingShells() > 0) {
        return Action::Shoot;
    }

    // Default fallback action
    return Action::Rotate1_8Right;
}

Movable::Direction Algorithm::calculateDirection(int currRow, int currCol, int targetRow, int targetCol) {
        if (currRow < targetRow && currCol == targetCol) return Movable::Direction::D;
        if (currRow > targetRow && currCol == targetCol) return Movable::Direction::U;
        if (currRow == targetRow && currCol < targetCol) return Movable::Direction::R;
        if (currRow == targetRow && currCol > targetCol) return Movable::Direction::L;
        if (currRow < targetRow && currCol < targetCol) return Movable::Direction::DR;
        if (currRow < targetRow && currCol > targetCol) return Movable::Direction::DL;
        if (currRow > targetRow && currCol < targetCol) return Movable::Direction::UR;
        if (currRow > targetRow && currCol > targetCol) return Movable::Direction::UL;

        return player->getDir(); // Default to current direction if no match
    }

Action Algorithm::determineRotation(Movable::Direction currentDir, Movable::Direction desiredDir) {
    int rotationSteps = (static_cast<int>(desiredDir) - static_cast<int>(currentDir) + 8) % 8;
    if (rotationSteps == 1 || rotationSteps == 2) return Action::Rotate1_8Right;
    if (rotationSteps == 6 || rotationSteps == 7) return Action::Rotate1_8Left;
    if (rotationSteps == 3) return Action::Rotate1_4Right;
    if (rotationSteps == 5) return Action::Rotate1_4Left;

    // If no rotation required
    return Action::Rotate1_8Right; // Arbitrary fallback
}
