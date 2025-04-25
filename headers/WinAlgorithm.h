#ifndef WINALGORITHM_H
#define WINALGORITHM_H

#include "Algorithm.h"

class Tank;
using namespace std;

struct RotationOption {
	Action action;
	int safetyScore;
	bool canMove;
	Direction newDir;
};

class WinAlgorithm : public Algorithm {
	vector<Action> rotations = {Action::Rotate1_8Left,Action::Rotate1_4Left, Action::Rotate1_8Right, Action::Rotate1_4Right};
public:
   	explicit WinAlgorithm(Tank* p):Algorithm(p){}
    Action nextMove(const OppData opp,const GameBoard& board);
 	~WinAlgorithm();
    Direction simulateRotation(Action act,const GameBoard& board);
    bool shouldShootOpponent(OppData opp,const GameBoard& board);
	bool willHaveTimeToShootAfterRotation(Action rotation, const GameBoard& board);

	int countOpenSpaceInDirection(const GameBoard& board, pair<int,int> pos);//(Direction dir, const GameBoard& board, int steps);
	Action calculateBestEscapeRotation(const GameBoard& board);
	RotationOption rotationOption(Action rotation, Direction newDir,Direction oldDir,const GameBoard& board);


};

#endif //WINALGORITHM_H