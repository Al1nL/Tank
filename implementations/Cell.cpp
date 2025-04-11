#include "Cell.h"
#include <new> // because of enum union use

Cell::Cell() : occupierType(OccupierType::None), passingShell(nullptr) {}
Cell::Cell(occupierType o) {
  if (o == OccupierType::Wall) {
    occupierType = OccupierType::Wall;
    new (&wall) Wall();
  }
  else if (o == OccupierType::Mine) {
    occupierType = OccupierType::Mine;
    new (&mine) Mine();
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

// Check if it has a tank
bool Cell::hasTank() const {
    return occupierType == OccupierType::Tank;
}

// Set tank
void Cell::setTank(Tank& newTank) {
    destroyOccupier(); // Clear previous object first
    new (&tank) Tank(newTank); // Placement new
    occupierType = OccupierType::Tank;
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

// Handle collision
void Cell::detectCollision(Shell* other) {
    delete other;
    passingShell = nullptr;
}