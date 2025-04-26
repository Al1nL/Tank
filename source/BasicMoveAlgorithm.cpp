#include "../headers/BasicMoveAlgorithm.h"

Action BasicMoveAlgorithm::nextMove(OppData opp,const GameBoard& board){

    if(!player->isWaitingToReverse() && player->getWaitingToReverse())
        return Action::MoveBack;
    if (willBeHitIn(player->getPos().first,player->getPos().second,1,board)) {
        Action res=checkForEscape(board);
        if(res!=Action::None)
          return res;
        return Action::Rotate1_8Right;
    }
    if (shouldShootOpponent(opp.opponentPos)) {
        return Action::Shoot;
    }
    if (canMoveFwd(board)) {
        return Action::MoveFwd;
    }
    return Action::Rotate1_8Right;
}