#include "../headers/Algorithm.h"

/**
 * @brief Calculates the general direction from the current position to the target position.
 *
 * @param currRow Current row position.
 * @param currCol Current column position.
 * @param targetRow Target row position.
 * @param targetCol Target column position.
 * @return Direction to move toward the target, or current direction if already at the target.
 */
Direction Algorithm::calculateDirection(int currRow, int currCol, int targetRow, int targetCol) {
	if (currRow < targetRow && currCol == targetCol) return Direction::D;
	if (currRow > targetRow && currCol == targetCol) return Direction::U;
	if (currRow == targetRow && currCol < targetCol) return Direction::R;
	if (currRow == targetRow && currCol > targetCol) return Direction::L;
	if (currRow < targetRow && currCol < targetCol) return Direction::DR;
	if (currRow < targetRow && currCol > targetCol) return Direction::DL;
	if (currRow > targetRow && currCol < targetCol) return Direction::UR;
	if (currRow > targetRow && currCol > targetCol) return Direction::UL;

	return player->getDir(); // Default to current direction if no match
}

/**
 * @brief Determines the rotation action needed to align from the current direction to the desired direction.
 *
 * @param currentDir Current facing direction.
 * @param desiredDir Target direction.
 * @return Action representing the rotation to apply.
 */
Action Algorithm::determineRotation(Direction currentDir, Direction desiredDir) {
	int rotationSteps = (static_cast<int>(desiredDir) - static_cast<int>(currentDir) + 8) % 8;
	if (rotationSteps == 1 || rotationSteps == 2) return Action::Rotate1_8Right;
	if (rotationSteps == 6 || rotationSteps == 7) return Action::Rotate1_8Left;
	if (rotationSteps == 3) return Action::Rotate1_4Right;
	if (rotationSteps == 5) return Action::Rotate1_4Left;

	// If no rotation required
	return Action::Rotate1_8Right; // Arbitrary fallback
}

/**
 * @brief Checks if a given board position is free of occupiers(Wall, Tank, Mine).
 *
 * @param pos Position to check.
 * @param board The current game board.
 * @return True if the cell is free, false otherwise.
 */
bool Algorithm::isOccupierFree(pair<int,int> pos, const GameBoard& board)  {
	OccupierType type = board.at(pos).getOccupierType();
	return type == OccupierType::None;
}

/**
 * @brief Determines if the player should shoot at the opponent based on alignment and available shells.
 *
 * @param opponentPos The opponent's position.
 * @return True if the player should shoot, false otherwise.
 */
bool Algorithm::shouldShootOpponent(const pair<int,int>& opponentPos)  {
	return player->getRemainingShells() > 0 &&
	       isAlignedWithOpponent(opponentPos) &&
	       !player->isWaitingToShoot();
}

/**
 * @brief Checks if the player can move forward safely.
 *
 * @param board The current game board.
 * @return True if moving forward is possible and safe, false otherwise.
 */
bool Algorithm::canMoveFwd(const GameBoard& board)  {
	auto [r, c] = player->nextStep(true, board.getHeight(), board.getWidth());
	return isOccupierFree({r, c}, board) && !willBeHitIn(r,c,1,board);
}

/**
 * @brief Checks if the player can move backward safely.
 *
 * @param board The current game board.
 * @return True if moving backward is possible and safe, false otherwise.
 */
bool Algorithm::canMoveBack(const GameBoard& board)  {
	if (!player->isValidMove(board, Action::MoveBack)) return false;
	pair<int,int> next = player->nextStep(false, board.getHeight(), board.getWidth());
	return canSafelyBack(next.first, next.second, board) && isOccupierFree(next, board);
}

/**
 * @brief Determines if moving backward would be safe for the player.
 *
 * @param backR Row after moving backward.
 * @param backC Column after moving backward.
 * @param board The current game board.
 * @return True if the move is safe, false otherwise.
 */
bool Algorithm::canSafelyBack(int backR, int backC, const GameBoard& board)  {
	auto [r,c]=player->getPos();
	// current position and backward cell are safe
	if (!player->getMovedBackwardLast()) {
		if (willBeHitIn(r, c, 1, board) || willBeHitIn(r, c, 2, board) ||willBeHitIn(backR, backC, 3, board)) {//
			return false; // Danger in current or backward position
		}
	}
	else {
		// If just moved back, only check immediate danger in next cell
		if (willBeHitIn(backR, backC, 1, board)) {
			return false;
		}
	}
	return true; // Safe to move backward
}

/**
 * @brief Chooses an escape action if the player is in danger.
 *
 * @param board The current game board.
 * @return Action::MoveFwd if forward is safe, Action::MoveBack if backward is safe, or Action::None if stuck.
 */
Action Algorithm::checkForEscape(const GameBoard& board) {
	if (canMoveFwd(board)) {
		return Action::MoveFwd;
	}
	if (canMoveBack(board)) {
		return Action::MoveBack;
	}
	return Action::None;
}

/**
 * @brief Predicts if a shell will hit a specific cell within a given number of game steps.
 *
 * @param row Row of the cell to check.
 * @param col Column of the cell to check.
 * @param t Number of game steps ahead.
 * @param board The current game board.
 * @return True if a shell will hit, false otherwise.
 */
bool Algorithm::willBeHitIn(int row, int col, int t, const GameBoard& board) {
	auto shells = board.getAllFiredShells();  // Get all fired shells
	for (Shell* s : shells) {
		auto [sr, sc] = s->getPos();
		Direction sd = s->getDir();
		int step = s->getSteps();
		for(int i =0; i<= step; i++) {
			auto [dr, dc] = offsets[static_cast<int>(sd)];

			// Calc forward `t` steps:
			int stepsAhead = (t-1)*2 + i;
			int pr = sr + dr * stepsAhead;
			int pc = sc + dc * stepsAhead;

			// Wraparound edges
			pr = player->wrap(pr, board.getHeight());
			pc = player->wrap(pc, board.getWidth());

			if (pr == row && pc == col) {
				return true;  // A shell will hit the cell by that time
			}
		}
	}
	return false;
}

/**
 * @brief Checks if the player is aligned (same row or same column) with the opponent.
 *
 * @param opponentPos Opponent's current position.
 * @return True if aligned, false otherwise.
 */
bool Algorithm::isAlignedWithOpponent(pair<int, int> opponentPos) {
	auto [r,c]=player->getPos();
	return (r == opponentPos.first || c == opponentPos.second );
}