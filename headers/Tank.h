#ifndef TANK_H
#define TANK_H
#include <utility>
#include "Movable.h"
#include "GameBoard.h"
#include "Shell.h"
using namespace std;

class Tank: public Movable {
    int id;
    Direction dir;
    int remainingShells = 16;
    int cooldown = 0; // after shooting
    bool waitingToReverse = false;
    int waitCounter = 0; // after requsting back movement
    vector<Shell*> firedShells;
public:
    Tank(){}  // Default constructor
    Tank(int id, pair<int> position) : id(id), pos(position) {
        dir = id==1 ? Direction::L : Direction::R;  // Assign initial direction based on id
    }
    enum class Action {MoveFwd, MoveBack, Rotate1_8Left, Rotate1_4Left,Rotate1_8Right,Rotate1_4Right, Shoot};
    abstract Action decideNextAction(const GameBoard&);

    void applyAction(Action, GameBoard&);
    void rotate(Direction,Action);
    void deleteShell(Shell*);
    bool isValidMove(const GameBoard& board,int row,int col);

    int getRemainingShells()  {return remainingShells;}
    int getCooldown() { return cooldown; }
    int getWaitCounter()  { return waitCounter; }
    int getWaitingToReverse() { return waitingToReverse; }
};
#endif