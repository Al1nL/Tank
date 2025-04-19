#ifndef CELL_H  // Header guard to prevent multiple inclusions
#define CELL_H
//#include <vector>
//#include "Shell.h"
// class Tank;
//class Shell;
//class Mine;
//class GameManager;
// #include "Tank.h"

#include <vector>
#include <utility> // For std::pair
#include "Shell.h" // Needed for Shell pointer
// Forward declarations
// class Tank;
class GameManager;
class Shell;

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
        bool mine;
        int tankId;
    };
    Shell* passingShell = nullptr;
public:
    // Cell(GameManager& gm);
    Cell(OccupierType o, GameManager& gm, int tankId = -1);
    // void destroyOccupier(){occupierType == OccupierType::None;}
    bool isWalkable() const;
    int getTank();
    void destroyOccupier(){occupierType= OccupierType::None, tankId = -1, wall=Wall(), mine=false;};
    OccupierType getOccupierType() const { return occupierType; }
    bool hasWall() const { return occupierType == OccupierType::Wall; }
    void setTank(int tank) { tankId=tank ,occupierType = OccupierType::Tank; }
	bool hasTank() const { return occupierType == OccupierType::Tank; }
    Shell* getShell() const;
    void setShell(Shell* shell);
    bool hasShell() const {return passingShell != nullptr;};
    void damageWall();
    bool WallIsGone() const; //returns true if wall is gone in the last step
//private:
    void detectCollision(Shell* other);

    ~Cell(){ }; //if (passingShell != nullptr)  delete passingShell;
};
#endif