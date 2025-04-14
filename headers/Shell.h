#ifndef SHELL_H
#define SHELL_H
#include <utility>
class GameBoard;
#include "GameManager.h"
class Cell;

class Shell : public Movable {
    Direction dir;
    int ownerID=-1;
    GameManager& game_manager;  // Reference to GameManager

public:
    Shell(pair<int,int> pos, Direction dir, int ownerID,GameManager& gm) : Movable(pos), dir(dir), ownerID(ownerID),game_manager(gm) {};
    Shell();
    void move(const GameBoard& board);
    void goThroughWall(const pair<Cell*,Cell*> cells);
	bool isValidMove(const GameBoard& board); //idk if needed?? i think only relevant for tanks
    int getOwnerID() const { return ownerID; }
};
#endif