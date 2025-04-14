#ifndef CELL_H  // Header guard to prevent multiple inclusions
#define CELL_H
#include <vector>
class Tank;
class Shell;
class Mine;
class GameManager;
#include "Tank.h"

using namespace std;

struct Wall {
    int wallHealth = 2;
    int getHealth() const { return wallHealth; }
    void damageWall() { wallHealth--; }
};

enum class OccupierType {
    None,
    Wall,
    Tank,
    Mine
};

class Cell{
    OccupierType occupierType; // Enum to track the current type
    GameManager& gameManager;  // Reference to GameManager
	pair<int,int> pos;
    union {
        Wall wall;
        Tank* tank;
        bool mine;
    };
    Shell* passingShell;
public:
    Cell(GameManager& gm);
    Cell(OccupierType o, GameManager& gm);
    void destroyOccupier(){occupierType == OccupierType::None;}
    bool isWalkable() const;
    Tank* getTank();
    OccupierType getOccupierType() const { return occupierType; }
    bool hasWall() const { return occupierType == OccupierType::Wall; }
    void setTank(Tank& t) { tank = &t; occupierType = OccupierType::Tank; }
	bool hasTank() const { return occupierType == OccupierType::Tank && tank != nullptr; }
    Shell* getShell() const;
    void setShell(Shell* shell);
    void damageWall();
    bool WallIsGone() const; //returns true if wall is gone in the last step
private:
    void detectCollision(Shell* other);
};
#endif