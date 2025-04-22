#ifndef WINALGORITHM_H
#define WINALGORITHM_H

#include "Algorithm.h"

class Tank;
using namespace std;

class WinAlgorithm : public Algorithm {
//	bool hasDirectView(pair<int, int> opponentPos,const GameBoard& board);
	bool isInDanger(const GameBoard& board);
	vector<pair<int, int>> getSafePositions(const GameBoard& board);
public:
   	explicit WinAlgorithm(Tank* p){ this->player = p;}
 	Action nextMove(pair<int,int> opponentPos,const GameBoard& board);
 	~WinAlgorithm();
};

#endif //WINALGORITHM_H
