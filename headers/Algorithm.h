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
};


#endif //ALGORITHM_H
