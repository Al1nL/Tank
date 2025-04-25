#ifndef TANK_H
#define TANK_H
#include <utility>
#include <vector>
#include <algorithm> //for find func
#include "Movable.h"
#include "Action.h"
#include "Shell.h"
#include "GameBoard.h"

// Forward declarations
class GameBoard;
class Algorithm;
class Shell;
struct OppData;

using namespace std;

class Tank: public Movable {
    int id;
    int remainingShells = 16;
    int shootCooldown = 0;
    bool waitingForBackward = false;
    int backwardCooldown = 0; // Number of turns left until backward move happens
    bool cancelBackward = false; // Whether the backward move was canceled
    bool movedBackwardLast = false; // to know if immediate next back is allowed
    vector<Shell*> firedShells;
    Algorithm* moveDecider = nullptr;
   // bool preparingToShoot=false; //used in Alg

public:
    Tank();  // Default constructor
    Tank(int id, pair<int,int> position);

    Action decideNextAction(const OppData& opp,const GameBoard& board);
    void rotate(Action);
    void deleteShell(Shell*);
    void addShell(int rows, int cols);
    bool isValidMove(const GameBoard& board,Action action);
    const vector<Shell*> getFiredShells() const { return firedShells; }
    int getRemainingShells()  const {return remainingShells;}
    int isWaitingToShoot() const { return shootCooldown>0; }
    int getBackwardCooldown()  const { return backwardCooldown; }
    int getWaitingToReverse() const { return waitingForBackward; }
    bool isWaitingToReverse() const { return waitingForBackward && backwardCooldown>0; }
    bool getMovedBackwardLast() const { return movedBackwardLast; }
    int getShootCooldown() const {return shootCooldown;}

    int getID() const { return id; }
    //setter
    void setWaitingForBackward(bool wait) { waitingForBackward = wait; }
    void setBackwardCooldown(int b) { backwardCooldown = b; }
    void setMovedBackwardLast(bool moved) { movedBackwardLast = moved; }
    void setShootCooldown(int s) { shootCooldown = s; }

    ~Tank();
};
#endif