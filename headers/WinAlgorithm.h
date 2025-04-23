#ifndef WINALGORITHM_H
#define WINALGORITHM_H

#include "Algorithm.h"

class Tank;
using namespace std;

class WinAlgorithm : public Algorithm {

public:
   	explicit WinAlgorithm(Tank* p):Algorithm(p){}
 	Action nextMove(pair<int,int> opponentPos,const GameBoard& board);
 	~WinAlgorithm();
};

#endif //WINALGORITHM_H
