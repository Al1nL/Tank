#include "../headers/Tank.h"
#include "../headers/WinAlgorithm.h"
#include "../headers/BasicMoveAlgorithm.h"
//void Tank::applyAction(Action action, GameBoard& board){
//    //assuming its valid move
//    switch(action){
//        case Action::MoveFwd:
//            if (waitingForBackward) {
//                waitingForBackward = false;
//                backwardCooldown = 0;
//                    // Tank stays in place! No forward move happens!
//            } else {
//                pos = this->nextStep(1, dir, board.getHeight(), board.getWidth());
//            }
//            movedBackwardLast = false;
//            break;
//        case Action::MoveBack:
//           if (movedBackwardLast || (backwardCooldown == 0 && waitingForBackward)) {
//                // Instant backward move
//                pos = this->nextStep(-1, dir, board.getHeight(), board.getWidth());
//                waitingForBackward=false;
//            }
//           else if (backwardCooldown == 0) {
//                // Start the backward move cooldown (waiting for 2 steps)
//                backwardCooldown = 2;
//                waitingForBackward=true;
//            }
//            movedBackwardLast = false;
//            break;
//        case Action::Rotate1_8Left:
//        case Action::Rotate1_8Right:
//        case Action::Rotate1_4Left:
//        case Action::Rotate1_4Right:
//            movedBackwardLast = false;
//            rotate(dir, action);
//            movedBackwardLast = false;
//            break;
//        case Action::Shoot:
//            if (shootCooldown == 0) {
//                firedShells.push_back(new Shell(this->nextStep(1, dir, board.getHeight(), board.getWidth()), dir, id,gameManager));
//                shootCooldown = 4;
//            }
//            movedBackwardLast = false;
//            break;
//    }
//    if(action != Action::Shoot)
//      shootCooldown--;
//}
Tank::Tank(int id, pair<int,int> position) : id(id), Movable(position, id==1 ? Direction::L : Direction::R,1) {
          // Assign initial direction based on id
        moveDecider = id==1 ?(Algorithm*) new WinAlgorithm(this) : (Algorithm*) new BasicMoveAlgorithm(this);
        //id == 1 ? : new WinAlgorithm(this)
}

void Tank::addShell(int rows, int cols){
    firedShells.push_back(new Shell({wrap(pos.first+offsets[dir].first,rows), wrap(pos.second+offsets[dir].second,cols)},dir,id));
}

void Tank::rotate(Action action) {
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
//    auto it = find(firedShells.begin(), firedShells.end(), shellToDelete);
//    if (it != firedShells.end()) {
        delete shellToDelete;
        firedShells.erase(remove(firedShells.begin(), firedShells.end(), shellToDelete), firedShells.end());
//    }
}

Action Tank::decideNextAction(const pair<int,int> opponentPos, const GameBoard& board) {
	return moveDecider->nextMove(opponentPos,board);
}

bool Tank::isValidMove(const GameBoard& board,Action action){
  if (isWaitingToReverse() && action != Action::MoveFwd)
        return false;

    pair<int,int> newPos;
    OccupierType occupier;

    switch (action) {
        case Action::Shoot:
          return !isWaitingToShoot() && getRemainingShells() > 0;
        case Action::MoveFwd:
        	newPos = nextStep(true,board.getHeight(), board.getWidth());
            occupier = board.at(newPos).getOccupierType();
			return occupier != OccupierType::Wall;
        case Action::MoveBack: //if not waiting for backward move - it's legal?
          	newPos = nextStep(false, board.getHeight(), board.getWidth());
            occupier = board.at(newPos).getOccupierType();
        	return !isWaitingToReverse() && occupier != OccupierType::Wall;
        case Action::Rotate1_8Left:
        case Action::Rotate1_4Left:
        case Action::Rotate1_8Right:
        case Action::Rotate1_4Right:
            return !isWaitingToReverse();  // Rotation is always valid
        default:
            return false;
    }
}


Tank::~Tank(){

//  auto end = ;
  for (auto it = firedShells.begin(); it != firedShells.end();) {
          // If we find the shell in the vector
        delete *it;              // Delete the shell (free memory)
        it = firedShells.erase(it);   // Remove the pointer from the vector

  }

  delete moveDecider;
}