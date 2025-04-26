#include "../headers/BasicMoveAlgorithm.h"

Action BasicMoveAlgorithm::nextMove(OppData opp,const GameBoard& board){

    if(!player->isWaitingToReverse() && player->getWaitingToReverse())
        return Action::MoveBack;
    // Try to avoid immediate danger
    if (willBeHitIn(player->getPos().first,player->getPos().second,1,board)) {
        Action res=checkForEscape(board);
        if(res!=Action::None)
          return res;
        return Action::Rotate1_8Right;
    }
    // Shoot if aligned with opp and have shells
    if (shouldShootOpponent(opp.opponentPos)) {
        return Action::Shoot;
    }
    if (canMoveFwd(board)) {
        return Action::MoveFwd;
    }
//    if (canMoveBack(board)) {
//        return Action::MoveBack;
//    }
    return Action::Rotate1_8Right;
}