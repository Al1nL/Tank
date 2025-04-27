#include "../headers/BasicMoveAlgorithm.h"

/**
 * @brief Decides the next action for the basic tank movement algorithm.
 *
 * The priority of decisions is:
 * 1. Complete a delayed backward move if ready.
 * 2. Escape if in immediate danger.
 * 3. Shoot if the opponent is aligned and the tank has shells.
 * 4. Move forward if the path is clear.
 * 5. Move backward if forward is blocked and backward is safe.
 * 6. Rotate as default if no movement is possible.
 *
 * @param opp Opponent data (position, and direction).
 * @param board The current game board.
 * @return The chosen Action for the next move.
 */
Action BasicMoveAlgorithm::nextMove(OppData opp,const GameBoard& board) {

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

    return Action::Rotate1_8Right;
}