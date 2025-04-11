#ifndef SHELL_H
#define SHELL_H
#include <utility>
class GameBoard;
class Shell : public Movable {
    Direction dir;
    int ownerID=-1;
public:
    Shell(pair<int> pos, Direction dir, int ownerID) : pos(pos), dir(dir), ownerID(ownerID) {};
    Shell(){};
    void move(const GameBoard& board);
    void goThroughWall(const GameBoard& board,int row,int col);
	bool isValidMove(const GameBoard& board);
};
#endif