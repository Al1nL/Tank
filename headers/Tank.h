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
struct OppData;

using namespace std;

class Tank: public Movable {
    int id;
    int remainingShells = 16;
    int shootCooldown = 0;
    bool waitingForBackward = false;
    int backwardCooldown = 0; // Number of turns left until backward move happens
    bool cancelBackward = false; // Whether the backward move was canceled
    bool movedBackwardLast = false; // Indicates immediate next back is allowed
    vector<Shell*> firedShells;
    Algorithm* moveDecider = nullptr;

public:
    Tank(int id, pair<int,int> position);
    ~Tank();

    Action decideNextAction(const OppData& opp,const GameBoard& board);
    void rotate(Action);
    bool isValidMove(const GameBoard& board,Action action);
    int getID() const { return id; }

    // Shells and shoot related functions
    void deleteShell(Shell*);
    void addShell(int rows, int cols);
    int getShootCooldown() const {return shootCooldown;}
    const vector<Shell*> getFiredShells() const { return firedShells; }
    int getRemainingShells()  const {return remainingShells;}
    bool isWaitingToShoot() const { return shootCooldown>0; }

    // Back movements related functions
    int getBackwardCooldown()  const { return backwardCooldown; }
    int getWaitingToReverse() const { return waitingForBackward; }
    bool getMovedBackwardLast() const { return movedBackwardLast; }
    bool isWaitingToReverse() const { return waitingForBackward && backwardCooldown>0; }

    // Setters
    void setWaitingForBackward(bool wait) { waitingForBackward = wait; }
    void setBackwardCooldown(int b) { backwardCooldown = b; }
    void setMovedBackwardLast(bool moved) { movedBackwardLast = moved; }
    void setShootCooldown(int s) { shootCooldown = s; }
};

#endif