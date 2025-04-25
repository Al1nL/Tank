//
// Created by admin on 4/20/2025.
//

#ifndef BASICMOVEALGORITHM_H
#define BASICMOVEALGORITHM_H
#include "Algorithm.h"
class Tank;
class GameBoard;
using namespace std;

class BasicMoveAlgorithm : public Algorithm {

public:
    explicit BasicMoveAlgorithm(Tank* p):Algorithm(p){}
    Action nextMove(OppData opp,const GameBoard& board);
    ~BasicMoveAlgorithm(){this->player = nullptr;};
};
#endif //BASICMOVEALGORITHM_H
