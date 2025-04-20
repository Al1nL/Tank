#ifndef TANK_H
#define TANK_H
#include <utility>
#include <vector>
#include "Movable.h"
#include "Action.h"
#include "Shell.h"

// Forward declarations
class GameBoard;
class GameManager;
class Algorithm;
// class WinAlgorithm;
class Shell;

using namespace std;

class Tank: public Movable {
    GameManager& gameManager;  // Reference to GameManager
    int id;
    int remainingShells = 16;
    int shootCooldown = 0;
    bool waitingForBackward = false;
    int backwardCooldown = 0; // Number of turns left until backward move happens
    bool cancelBackward = false; // Whether the backward move was canceled
    bool movedBackwardLast = false; // to know if immediate next back is allowed
    vector<Shell*> firedShells;
    Algorithm* moveDecider = nullptr;

public:
    Tank();  // Default constructor
    Tank(int id, pair<int,int> position,GameManager& gm);

    Action decideNextAction(const pair<int,int> opponentPos,const GameBoard& board);

//    void applyAction(Action, GameBoard&);
    void rotate(Action);
    void deleteShell(Shell*);
    void addShell();
    bool isValidMove(const GameBoard& board,Action action);
    vector<Shell*> getFiredShells() const { return firedShells; }
    int getRemainingShells()  const {return remainingShells;}
    int isWaitingToShoot() { return shootCooldown>0; }
    int getbBckwardCooldown()  const { return backwardCooldown; }
    bool isWaitingToReverse() const { return waitingForBackward; }
    bool getMovedBackwardLast() const { return movedBackwardLast; }
    int getShootCooldown() const {return shootCooldown;}
    int getID() const { return id; }
    // void moveFiredShells(GameBoard& board);

    //setter
    void setWaitingForBackward(bool wait) { waitingForBackward = wait; }
    void setBackwardCooldown(int b) { backwardCooldown = b; }
    void setMovedBackwardLast(bool moved) { movedBackwardLast = moved; }
    void setShootCooldown(int s) { shootCooldown = s; }

    ~Tank();
};
#endif