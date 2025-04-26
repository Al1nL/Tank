#include "../headers/Cell.h"

/**
 * @brief Constructor
 *
 * Depending on the OccupierType:
 * - Initializes a Wall if it's a wall.
 * - Sets mine flag if it's a mine.
 * - Stores the tank ID if it's a tank.
 *
 * @param o The type of occupier (Wall, Mine, Tank, etc.).
 * @param p The (row, column) position of the cell.
 * @param tank The ID of the tank occupying the cell (if any).
 */
Cell::Cell(OccupierType o,pair<int,int> p, int tank) {
  occupierType = o;
  pos = p;
  if (o == OccupierType::Wall) {
    new (&wall) Wall();
  }
  else if (o == OccupierType::Mine) {
    mine=true;
  }
  else if(o == OccupierType::Tank) {
    tankId=tank;
  }
}

/**
 * @brief Damages the wall in the cell if present.
 *
 * If the wall's health reaches zero after damage, the wall is destroyed and the cell is freed.
 */
void Cell::damageWall() {
  if (occupierType == OccupierType::Wall) {
    if (wall.getHealth() > 0) {
      wall.damageWall();
    } else {
      destroyOccupier(); // Wall destroyed
    }
  }
}

/**
 * @brief Returns the ID of the tank occupying this cell.
 *
 * @return int Tank ID if a tank is present, -1 otherwise.
 */
int Cell::getTank() {
  if (hasTank()) {
    return tankId;
  }
  return -1;
}