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

    // Helper methods for nextMove
    // Rotation and safe choice related
    Direction simulateRotation(Action act,const GameBoard& board);
	Action calculateBestEscapeRotation(const GameBoard& board);
	int countOpenSpaceInDirection(const GameBoard& board, pair<int,int> pos);//(Direction dir, const GameBoard& board, int steps);
	RotationOption rotationOption(Action rotation, Direction newDir,Direction oldDir,const GameBoard& board);

    // Shoot related
	bool shouldShootOpponent(OppData opp,const GameBoard& board);
	bool willHaveTimeToShootAfterRotation(Action rotation, const GameBoard& board);
	bool canShootAfterRotate(Direction targetDir, const GameBoard& board, OppData opp);
	Direction calculateRealDirection(int currRow, int currCol, int targetRow, int targetCol);

    public:
   	explicit WinAlgorithm(Tank* p):Algorithm(p) {}
 	~WinAlgorithm() override;

	Action nextMove(const OppData opp,const GameBoard& board);
};

#endif //WINALGORITHM_H