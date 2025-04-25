#ifndef WINALGORITHM_H
#define WINALGORITHM_H

#include "Algorithm.h"

class Tank;
using namespace std;

class WinAlgorithm : public Algorithm {
	vector<Action> rotations = {Action::Rotate1_8Left,Action::Rotate1_4Left, Action::Rotate1_8Right, Action::Rotate1_4Right};
public:
   	explicit WinAlgorithm(Tank* p):Algorithm(p){}
    Action nextMove(const OppData opp,const GameBoard& board);
 	~WinAlgorithm();
    Direction simulateRotation(Action act,const GameBoard& board);
    bool shouldShootOpponent(OppData opp,const GameBoard& board);
	bool willHaveTimeToShootAfterRotation(Action rotation, const GameBoard& board);

	int countOpenSpaceInDirection(Direction dir, const GameBoard& board, int steps);
	Action calculateBestEscapeRotation(const GameBoard& board);



};

#endif //WINALGORITHM_H