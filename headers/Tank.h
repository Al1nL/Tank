#ifndef TANK_H
#define TANK_H
#include <utility>
#include "Movable.h"
#include "GameBoard.h"
#include "Shell.h"
#include "Action.h"
class GameManager;
using namespace std;

class Tank: public Movable {
    GameManager& gameManager;  // Reference to GameManager
    int id;
    Direction dir;
    int remainingShells = 16;
    int shootCooldown = 0;
    bool waitingForBackward = false;
    int backwardCooldown = 0; // Number of turns left until backward move happens
    bool cancelBackward = false; // Whether the backward move was canceled
    bool movedBackwardLast = false; // to know if immediate next back is allowed
    vector<Shell*> firedShells;

public:
    Tank();  // Default constructor
    Tank(int id, pair<int,int> position,GameManager& gm) : id(id), Movable(position),gameManager(gm) {
        dir = id==1 ? Direction::L : Direction::R;  // Assign initial direction based on id
    }
    Action decideNextAction(const GameBoard&);

    void applyAction(Action, GameBoard&);
    void rotate(Direction,Action);
    void deleteShell(Shell*);
    bool isValidMove(const GameBoard& board,int row,int col);
    vector<Shell*> getFiredShells() const { return firedShells; }
    int getRemainingShells()  {return remainingShells;}
    int isWaitingToShoot() { return shootCooldown>0; }
    int getbBckwardCooldown()  { return backwardCooldown; }
    bool isWaitingToReverse() { return waitingForBackward; }
    int getID() const { return id; }
};
#endif