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
   	explicit WinAlgorithm(Tank* p):Algorithm(p){}
 	Action nextMove(pair<int,int> opponentPos,const GameBoard& board);
 	~WinAlgorithm();

        //test
        bool canSafelyBack(int backR, int backC, const GameBoard& board);
        bool isAlignedWithOpponent(pair<int, int> opponentPos);
        bool isWalkable(int r, int c, const GameBoard& board);
        bool willBeHitIn(int row, int col, int t, const GameBoard& board);

};

#endif //WINALGORITHM_H
