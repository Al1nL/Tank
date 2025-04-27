#ifndef SHELL_H
#define SHELL_H
#include <utility>
#include "Movable.h"

class GameBoard;

class Shell : public Movable {
    int ownerID=-1;
    pair<int,int> offset;
    bool isNew=true;

public:
    Shell(pair<int,int> pos, Direction dir, int ownerID) : Movable(pos,dir,2), ownerID(ownerID) {
        offset = offsets[dir];
    };
    ~Shell()=default;

    // Getters
    int getOwnerID() const { return ownerID; }
    const pair<int,int> getOffset() const { return offset; }

    // Just made shell indicator
    bool isNewShell() const {return isNew;}
    void setNotNewShell() {isNew=false;}
};

#endif