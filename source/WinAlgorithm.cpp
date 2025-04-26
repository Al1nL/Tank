#include "../headers/WinAlgorithm.h"

Action WinAlgorithm::nextMove(const OppData opp,const GameBoard& board) {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opp.opponentPos;

    Direction currentDir = player->getDir();
    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    if(!player->isWaitingToReverse() && player->getWaitingToReverse())
         return Action::MoveBack;

    // Try to dodge danger
    if (willBeHitIn(player->getPos().first,player->getPos().second,1,board)) {
		Action res=checkForEscape(board);
        if(res!=Action::None)
          return res;
        return Action::Shoot;
    }
    if(willBeHitIn(player->getPos().first,player->getPos().second,2,board)) {
		Action res;
        res =calculateBestEscapeRotation(board);
        if(res!=Action::None)
          return res;
        else{
          	res=checkForEscape(board);
        	if(res!=Action::None)
          		return res;
        	for(Action rotation: rotations)
	        	if(willHaveTimeToShootAfterRotation(rotation,board))
    	        	return rotation;
        }
    }

    // Shoot if aligned and safe
    if ((shouldShootOpponent(opp, board)) &&  !player->isWaitingToShoot()) { //(currentDir == desiredDir
        return Action::Shoot;
    }
    if (currentDir != desiredDir) {
         Action rotate= determineRotation(currentDir, desiredDir);
         RotationOption option = rotationOption(rotate, desiredDir, currentDir ,board);
         if(option.canMove || canShootAfterRotate(desiredDir,board, opp) && option.safetyScore > 0)
         	return rotate;
    }

    // Move forward if not aligned/blocked
    pair<int,int> next = player->nextStep(true, board.getHeight(), board.getWidth());
    if ((currentRow != targetRow || currentCol != targetCol) && isOccupierFree(next, board)) {
        return Action::MoveFwd;
    }

//    if (canMoveBack(board)) {
//        return Action::MoveBack;
//    }
    // Default fallback action
    Action r =calculateBestEscapeRotation(board);
    return r == Action::None ? Action::Rotate1_8Right : r;
}

WinAlgorithm::~WinAlgorithm() {
    player = nullptr;
}

Direction WinAlgorithm::simulateRotation(Action act,const GameBoard& board) {
    Direction currentDir = player->getDir();
    player->rotate(act);
    Direction newDir = player->getDir();
    if(canMoveFwd(board)){
        player->setDir(currentDir);
        return newDir;
    }
    return Direction::None;
}

bool WinAlgorithm::shouldShootOpponent(OppData opp, const GameBoard& board) {
    if (player->isWaitingToShoot() || player->getRemainingShells() <= 0) {
        return false;
    }

    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opp.opponentPos;

    // Basic opponent always moves forward when possible, else rotates right
    Direction oppDir = opp.opponentDir;
    auto [dr, dc] = offsets[static_cast<int>(oppDir)];

    // Predict opponent's next 2 positions
    vector<pair<int,int>> predictedPositions = {
        {targetRow, targetCol},  // Current position
        {player->wrap(targetRow + dr, board.getHeight()), player->wrap(targetCol + dc, board.getWidth())} // Next position
        ,{player->wrap(targetRow + dr*2, board.getHeight()), player->wrap(targetCol + dc*2, board.getWidth())} // Position after next
    };

    // Check if we're aligned with any predicted position
    for (const auto& [r, c] : predictedPositions) {
        if (player->getDir() == calculateRealDirection(currentRow, currentCol, r, c)) {
            return true;
        }
    }
    return false;
}

bool WinAlgorithm::willHaveTimeToShootAfterRotation(Action rotation, const GameBoard& board) {
	Direction d=simulateRotation(rotation, board);
    // Check if we'll be able to shoot next turn
    if (player->isWaitingToShoot() && player->getShootCooldown() > 1) {
        return false; // Still in cooldown
    }

    // Verify the threat will still be there after rotating
    auto [row, col] = player->getPos();
    for (Shell* shell : board.getAllFiredShells()) {
        auto [sRow, sCol] = shell->getPos();
        Direction sDir = shell->getDir();


        // Calculate shell position after rotation (1 step)
        int shellRow = sRow;
        int shellCol = sCol;
        auto [sDr, sDc] = offsets[static_cast<int>(sDir)];
        shellRow = player->wrap(shellRow + sDr, board.getHeight());
        shellCol = player->wrap(shellCol + sDc, board.getWidth());

        // Check alignment after rotation
        if ((row == shellRow || row == shellCol) &&
            d == calculateDirection(row, col, shellRow, shellCol)) {
            return true;
        }
    }
    return false;
}

// New helper function
bool WinAlgorithm::canShootAfterRotate(Direction targetDir, const GameBoard& board, OppData opp) {
    // Simulate rotation
    Direction currentDir = player->getDir();
    player->setDir(targetDir);

    // Check if we could shoot after rotating
    bool canShoot = shouldShootOpponent(opp, board);

    // Restore direction
    player->setDir(currentDir);
    return canShoot;
}

Action WinAlgorithm::calculateBestEscapeRotation(const GameBoard& board) {
    Direction currentDir = player->getDir();
    auto [row, col] = player->getPos();

    vector<RotationOption> options;
	RotationOption option;
    // Evaluate each rotation option
    for (Action rotation : rotations) {
        // Simulate rotation
        player->rotate(rotation);
        Direction newDir = player->getDir();

		option = rotationOption(rotation, newDir, currentDir, board);
        options.push_back(option);
    }

    // Sort options by safety score (descending)
    sort(options.begin(), options.end(), [](const RotationOption& a, const RotationOption& b) {
        if (a.safetyScore == b.safetyScore) {
            return a.canMove > b.canMove ? true : a.action != Action::Rotate1_8Right; // Prefer options that allow movement
        }
        return a.safetyScore > b.safetyScore;
    });

    return options.empty() ? Action::None : options[0].action;
}

RotationOption WinAlgorithm::rotationOption(Action rotation, Direction newDir,Direction oldDir,const GameBoard& board) {
  		RotationOption option;
        option.action = rotation;
        option.newDir = newDir;
        player->setDir(newDir);
        option.canMove = canMoveFwd(board);
        // Calculate safety score (higher is better)
        option.safetyScore = 0;

        // Check immediate safety
        auto [nextRow, nextCol] = player->nextStep(true, board.getHeight(), board.getWidth());
        if (!willBeHitIn(nextRow, nextCol, 1, board)) option.safetyScore += 2;
        if (!willBeHitIn(nextRow, nextCol, 2, board)) option.safetyScore += 1;

        // Bonus for moving towards open space
        if (option.canMove) {
            option.safetyScore += 2;

            // Check if this direction leads to more open space
            int openSpace = countOpenSpaceInDirection(board, {nextRow, nextCol});
            option.safetyScore += openSpace;
        }
         // Reset direction
        player->setDir(oldDir);
        return option;
}

int WinAlgorithm::countOpenSpaceInDirection(const GameBoard& board, pair<int,int> pos) {
  	auto [row, col] = pos;
    int openCount = 0;
  	for(auto [dr, dc] : offsets){
          int newRow = player->wrap(row + dr, board.getHeight());
        int newCol = player->wrap(col + dc, board.getWidth());

        if (isOccupierFree({newRow, newCol}, board)){ //&&!willBeHitIn(newRow, newCol, i, board)) {
            openCount++;
         }
  	}
        return openCount;
  }

 Direction WinAlgorithm::calculateRealDirection(int currRow, int currCol, int targetRow, int targetCol) {
    int rowDiff = targetRow - currRow;
    int colDiff = targetCol - currCol;

    // Normalize differences to identify direction only if strictly aligned
    if (rowDiff > 0 && colDiff == 0) return Direction::D;    // Down
    if (rowDiff < 0 && colDiff == 0) return Direction::U;    // Up
    if (rowDiff == 0 && colDiff > 0) return Direction::R;    // Right
    if (rowDiff == 0 && colDiff < 0) return Direction::L;    // Left
    if (rowDiff == colDiff && rowDiff > 0) return Direction::DR; // Down-Right (strict diagonal)
    if (rowDiff == -colDiff && rowDiff > 0) return Direction::DL; // Down-Left (strict diagonal)
    if (rowDiff == colDiff && rowDiff < 0) return Direction::UR; // Up-Right (strict diagonal)
    if (rowDiff == -colDiff && rowDiff < 0) return Direction::UL; // Up-Left (strict diagonal)

    return Direction::None; // None for non-aligned movement
 }