#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <utility>
#include <queue>
#include "Action.h"
#include "Tank.h"

class Tank;
using namespace std;


class Algorithm: public Movable {

protected:
	Tank* player;

public:
	Algorithm() : player(nullptr) {}
    Algorithm(Tank* p) : player(p),Movable(p->getPos(),p->getDir(),1) {}
	virtual Action nextMove(pair<int,int> opponentPos,const GameBoard& board) = 0;

	virtual ~Algorithm()= default;
	Direction calculateDirection(int currRow, int currCol, int targetRow, int targetCol);
	Action determineRotation(Direction currentDir, Direction desiredDir);
	// Common decision utilities
	bool isOccupierFree(std::pair<int, int> pos, const GameBoard& board) ;
	bool shouldShootOpponent(const std::pair<int, int>& opponentPos) ;
	bool canMoveFwd(const GameBoard& board) ;
	bool canMoveBack(const GameBoard& board) ;

protected:
	// Safety check for backward movement
	bool canSafelyBack(int backR, int backC, const GameBoard& board);
	bool willBeHitIn(int row, int col, int t, const GameBoard& board);
    bool isInDanger(const GameBoard& board);
    bool isAlignedWithOpponent(pair<int, int> opponentPos) ;
};


#endif //ALGORITHM_H
