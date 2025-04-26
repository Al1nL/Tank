#ifndef CELL_H  // Header guard to prevent multiple inclusions
#define CELL_H

#include <vector>
#include <utility> // For std::pair
#include "Shell.h" // Needed for Shell pointer
// Forward declarations
// class Tank;
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
	pair<int,int> pos;
    union {
        Wall wall;    // Only valid when occupierType == Wall
        bool mine;    // Only valid when occupierType == Mine
        int tankId;   // Only valid when occupierType == Tank
    };
    Shell* passingShell = nullptr; // Shell currently passing through this cell
public:
    Cell(OccupierType o,pair<int,int> pos, int tankId = -1);
    ~Cell()=default;

    // State check
    bool isWalkable() const{return occupierType == OccupierType::None;}
    bool hasWall() const { return occupierType == OccupierType::Wall; }
    bool hasTank() const { return occupierType == OccupierType::Tank; }
    bool hasShell() const {return passingShell != nullptr;};

    // Getters
    int getTank();
    const pair<int,int> getPos() const {return pos;}
    const OccupierType getOccupierType() const { return occupierType; }
    const Shell* getShell() const{return passingShell;}

    // Setters
    void destroyOccupier(){occupierType= OccupierType::None, tankId = -1, wall=Wall(), mine=false;};
    void setTank(int tank) { tankId=tank ;occupierType = OccupierType::Tank; }
	void setShell(Shell* shell){passingShell = shell;}

    void damageWall();
    void detectCollision(Shell* other);
};

#endif