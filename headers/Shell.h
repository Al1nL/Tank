#ifndef SHELL_H
#define SHELL_H
#include <utility>
class GameBoard;
#include "GameManager.h"
#include "Movable.h"
#include "GameBoard.h"
class Cell;

class Shell : public Movable {
    int ownerID=-1;
    pair<int,int> offset;
//    GameManager& game_manager;  // Reference to GameManager

public:
    Shell(pair<int,int> pos, Direction dir, int ownerID) : Movable(pos,dir,2), ownerID(ownerID) {
        offset = offsets[dir];
    };
    Shell();
    ~Shell(){};

    // void move(GameBoard& board);
    // bool goThroughCells(const vector<Cell*> cells);
	bool isValidMove(const GameBoard& board); //idk if needed?? i think only relevant for tanks
    int getOwnerID() const { return ownerID; }
    pair<int,int> getOffset() const { return offset; }
};
#endif