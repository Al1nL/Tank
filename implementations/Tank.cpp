#include "Tank.h"

void Tank::applyAction(Action action, GameBoard& board){
    //assuming its valid move
    switch(action){
        case Action::MoveFwd:
            if (waitingForBackward) {
                waitingForBackward = false;
                backwardCooldown = 0;
                    // Tank stays in place! No forward move happens!
            } else {
                pos = this->nextStep(1, dir, board.getHeight(), board.getWidth());
            }
            movedBackwardLast = false;
            break;
        case Action::MoveBack:
           if (movedBackwardLast || (backwardCooldown == 0 && waitingForBackward)) {
                // Instant backward move
                pos = this->nextStep(-1, dir, board.getHeight(), board.getWidth());
                waitingForBackward=false;
            }
           else if (backwardCooldown == 0) {
                // Start the backward move cooldown (waiting for 2 steps)
                backwardCooldown = 2;
                waitingForBackward=true;
            }
            movedBackwardLast = false;
            break;
        case Action::Rotate1_8Left:
        case Action::Rotate1_8Right:
        case Action::Rotate1_4Left:
        case Action::Rotate1_4Right:
            movedBackwardLast = false;
            rotate(dir, action);
            movedBackwardLast = false;
            break;
        case Action::Shoot:
            if (shootCooldown == 0) {
                firedShells.push_back(new Shell(this->nextStep(1, dir, board.getHeight(), board.getWidth()), dir, id,gameManager));
                shootCooldown = 4;
            }
            movedBackwardLast = false;
            break;
    }
    if(action != Action::Shoot)
      shootCooldown--;
}


void Tank::rotate(Movable::Direction dir, Action action) {
    switch(action) {
        case Action::Rotate1_8Left:
            dir = static_cast<Direction>((dir + 7) % 8); // move 1 counter-clockwise
        break;
        case Action::Rotate1_8Right:
            dir = static_cast<Direction>((dir + 1) % 8); // move 1 clockwise
        break;
        case Action::Rotate1_4Left:
            dir = static_cast<Direction>((dir + 6) % 8); // move 2 clockwise
        break;
        case Action::Rotate1_4Right:
            dir = static_cast<Direction>((dir + 2) % 8); // move 2 counter-clockwise
        break;
        case Action::MoveFwd:
        case Action::MoveBack:
        case Action::Shoot:
            break;
    }
}

void Tank::deleteShell(Shell* shellToDelete) {
    // Iterate over the firedShells vector
    for (auto it = firedShells.begin(); it != firedShells.end(); ++it) {
        if (*it == shellToDelete) {  // If we find the shell in the vector
            delete *it;              // Delete the shell (free memory)
            firedShells.erase(it);   // Remove the pointer from the vector
            break;                    // Exit the loop as we found and deleted the shell
        }
    }
}
