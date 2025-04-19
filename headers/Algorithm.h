#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <utility>
#include <queue>
#include "Action.h"
#include "Tank.h"

class Tank;
using namespace std;


class Algorithm {

protected:
	Tank* player;

public:
	Algorithm() : player(nullptr) {};
	// Algorithm(Tank* p) : player(p) {}
	virtual Action nextMove(pair<int,int> opponentPos,const GameBoard& board) = 0;

	virtual ~Algorithm()= default;
	Direction calculateDirection(int currRow, int currCol, int targetRow, int targetCol);
	Action determineRotation(Direction currentDir, Direction desiredDir);
};


// class WinAlgorithm : public Algorithm {
//
// 	vector<pair<int, int>> getNeighbors(int x, int y);
// public:
// 	WinAlgorithm(Tank* p): Algorithm() { this->player = p;}
// 	Action nextMove(pair<int,int> opponentPos) override;
// 	~WinAlgorithm() override = default;
// };


#endif //ALGORITHM_H
