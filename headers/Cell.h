#ifndef CELL_H  // Header guard to prevent multiple inclusions
#define CELL_H
#include <vector>
class Tank;
class Shell;
class Mine;
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
    union {
        Wall wall;
        Tank tank;
        Mine mine;
    };
    Shell* passingShell;
public:
    Cell();
    Cell(OccupierType o);
    ~Cell();
    void destroyOccupier();
    bool isWalkable() const;
    bool hasTank() const;
    void setTank(Tank& tank);
    Shell* getShell() const;
    void setShell(Shell* shell);
    void damageWall();
private:
    void detectCollision(shell* other);
};
#endif