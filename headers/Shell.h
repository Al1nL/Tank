#ifndef SHELL_H
#define SHELL_H
#include <utility>
class GameBoard;
#include "Movable.h"
class Cell;

class Shell : public Movable {
    int ownerID=-1;
    pair<int,int> offset;
    bool isNew=true;

public:
    Shell(pair<int,int> pos, Direction dir, int ownerID) : Movable(pos,dir,2), ownerID(ownerID) {
        offset = offsets[dir];
    };
    Shell();
    ~Shell(){};

    // void move(GameBoard& board);
    // bool goThroughCells(const vector<Cell*> cells);
    int getOwnerID() const { return ownerID; }
    bool isNewShell() {return isNew;}
    void setNotNewShell() {isNew=false;}
    pair<int,int> getOffset() const { return offset; }
};
#endif