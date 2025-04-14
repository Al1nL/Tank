//
// Created by admin on 4/14/2025.
//

#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <vector>
#include <utility>
#include <queue>
#include "Action.h"
#include "Tank.h"


using namespace std;
class Algorithm {
  	protected:
    	Tank* player;
        Tank* opponent;
	public:
    	Algorithm(){};
        Algorithm(Tank* player, Tank* opponent): player(player), opponent(opponent){}
    	virtual Action nextMove() = 0;
        Movable::Direction calculateDirection(int currRow, int currCol, int targetRow, int targetCol);
		Action determineRotation(Movable::Direction currentDir, Movable::Direction desiredDir);
};

class WinAlgorithm : public Algorithm {
    private:
        vector<pair<int, int>> getNeighbors(int x, int y);
    public:
        WinAlgorithm(){};
        WinAlgorithm(Tank* player, Tank* opponent): Algorithm(player, opponent){};
        Action nextMove() override;
};



#endif //ALGORITHM_H
