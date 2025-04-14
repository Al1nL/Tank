#include "../headers/Cell.h"

Cell::Cell(GameManager& gameManager) : occupierType(OccupierType::None), passingShell(nullptr),gameManager(gameManager) {

}

Cell::Cell(OccupierType o,GameManager& gameManager) : gameManager(gameManager) {
  if (o == OccupierType::Wall) {
    occupierType = OccupierType::Wall;
    new (&wall) Wall();
  }
  else if (o == OccupierType::Mine) {
    occupierType = OccupierType::Mine;
    mine=true;
  }
}

void Cell::damageWall() {
    if (occupierType == OccupierType::Wall) {
        if (wall.getHealth() > 0) {
            wall.damageWall();
            gameManager.logWallWeakened(pos);
        } else {
            destroyOccupier(); // Wall destroyed
            gameManager.logWallDestroyed(pos);
        }
    }
}

// Check if walkable
bool Cell::isWalkable() const {
    return occupierType == OccupierType::None;
}

// Get shell
Shell* Cell::getShell() const {
    return passingShell;
}

// Set shell
void Cell::setShell(Shell* s) {
    if (passingShell == nullptr)
        passingShell = s;
    else
        detectCollision(s);
}

Tank* Cell::getTank() {
  if (hasTank()) {
    return tank;
  }
    return nullptr;

}
// Handle collision
//void Cell::detectCollision(Shell* other) {
//    gameManager.logShellsCollided(*passingShell, *other);
//    passingShell = nullptr;
//}

void Cell::detectCollision(Shell* other) {
    gameManager.logShellsCollided(*passingShell, *other);
    delete passingShell; // Clean up current shell
    delete other;        // Clean up colliding shell
    passingShell = nullptr;
}