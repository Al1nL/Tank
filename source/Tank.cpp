#include "../headers/Tank.h"
#include "../headers/WinAlgorithm.h"
#include "../headers/BasicMoveAlgorithm.h"

/**
 * @brief Constructor.
 *
 * Initializes direction and movement algorithm based on id.
 * @param id Tank's ID (1 or 2).
 * @param position Initial (row, column) position.
 */
Tank::Tank(int id, pair<int,int> position) : id(id), Movable(position, id==1 ? Direction::L : Direction::R,1) {
	// Assign initial game algo and direction based on id
	moveDecider = id==1 ?(Algorithm*) new WinAlgorithm(this) : (Algorithm*) new BasicMoveAlgorithm(this);
}

/**
 * @brief Fires a new shell in the current direction.
 *
 * @param rows Total number of board rows.
 * @param cols Total number of board columns.
 */
void Tank::addShell(int rows, int cols) {
	firedShells.push_back(new Shell(nextStep(true,rows,cols),dir,id));
}

/**
 * @brief Rotates the tank according to the given action.
 *
 * @param action Rotation action.
 */
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

/**
 * @brief Deletes a fired shell and removes it from the list.
 *
 * @param shellToDelete Pointer to the shell to delete(destroyed).
 */
void Tank::deleteShell(Shell* shellToDelete) {
	delete shellToDelete;
	firedShells.erase(remove(firedShells.begin(), firedShells.end(), shellToDelete), firedShells.end());
}

/**
 * @brief Decides the next action for the tank based on opponent and board state.
 *
 * @param opp Contains opponents position and direction.
 * @param board The current game board.
 * @return The next decided Action.
 */
Action Tank::decideNextAction(const OppData& opp, const GameBoard& board) {
	return moveDecider->nextMove(opp,board);
}

/**
 * @brief Checks if the given action is a valid move in the current state.
 *
 * @param board The current game board.
 * @param action The action to validate.
 * @return True if the action is valid, false otherwise.
 */
bool Tank::isValidMove(const GameBoard& board,Action action) {
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

/**
 * @brief Destructor.
 */
Tank::~Tank() {
	for (Shell* shell : firedShells)
		delete shell;

	firedShells.clear();
	delete moveDecider;
}