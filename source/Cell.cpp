#include "../headers/Cell.h"

//Cell::Cell(GameManager& gameManager) : occupierType(OccupierType::None), passingShell(nullptr),gameManager(gameManager) {
//
//}

Cell::Cell(OccupierType o, int tank){
  occupierType = o;
  if (o == OccupierType::Wall) {
    new (&wall) Wall();
  }
  else if (o == OccupierType::Mine) {
    mine=true;
  }else if(o == OccupierType::Tank){
    tankId=tank;
  }
}

void Cell::damageWall() {
    if (occupierType == OccupierType::Wall) {
        if (wall.getHealth() > 0) {
            wall.damageWall();
        } else {
            destroyOccupier(); // Wall destroyed
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

int Cell::getTank() {
  if (hasTank()) {
    return tankId;
  }
  return -1;

}

// Handle collision
//void Cell::detectCollision(Shell* other) {
//    gameManager.logShellsCollided(*passingShell, *other);
//    passingShell = nullptr;
//}

void Cell::detectCollision(Shell* other) {
    delete passingShell; // Clean up current shell
    delete other;        // Clean up colliding shell
    passingShell = nullptr;
}