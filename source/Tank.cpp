#include "../headers/Tank.h"
#include "../headers/WinAlgorithm.h"
#include "../headers/BasicMoveAlgorithm.h"

Tank::Tank(int id, pair<int,int> position) : id(id), Movable(position, id==1 ? Direction::L : Direction::R,1) {
    // Assign initial direction based on id
    moveDecider = id==1 ?(Algorithm*) new WinAlgorithm(this) : (Algorithm*) new BasicMoveAlgorithm(this);
}

void Tank::addShell(int rows, int cols){
    firedShells.push_back(new Shell(nextStep(true,rows,cols),dir,id));
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
    delete shellToDelete;
    firedShells.erase(remove(firedShells.begin(), firedShells.end(), shellToDelete), firedShells.end());
}

Action Tank::decideNextAction(const OppData& opp, const GameBoard& board) {
	return moveDecider->nextMove(opp,board);
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
        case Action::MoveBack:
          	newPos = nextStep(false, board.getHeight(), board.getWidth());
            occupier = board.at(newPos).getOccupierType();
        	return !isWaitingToReverse() && occupier != OccupierType::Wall;
        case Action::Rotate1_8Left:
        case Action::Rotate1_4Left:
        case Action::Rotate1_8Right:
        case Action::Rotate1_4Right:
            return !isWaitingToReverse();
        default:
            return false;
    }
}

Tank::~Tank(){
  for (auto it = firedShells.begin(); it != firedShells.end();) {
          // If we find the shell in the vector
        delete *it;              // Delete the shell
        it = firedShells.erase(it);   // Remove the pointer from the vector
  }
  delete moveDecider;
}