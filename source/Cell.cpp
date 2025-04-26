#include "../headers/Cell.h"

Cell::Cell(OccupierType o,pair<int,int> poss, int tank){
  occupierType = o;
  pos = poss;
  if (o == OccupierType::Wall) {
    new (&wall) Wall();
  }
  else if (o == OccupierType::Mine) {
    mine=true;
  }
  else if(o == OccupierType::Tank){
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

int Cell::getTank() {
  if (hasTank()) {
    return tankId;
  }
  return -1;
}

void Cell::detectCollision(Shell* other) {
    delete passingShell; // Clean up current shell
    delete other;        // Clean up colliding shell
    passingShell = nullptr;
}