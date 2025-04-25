//
// Created by admin on 4/17/2025.
//
#include "../headers/WinAlgorithm.h"

Action WinAlgorithm::nextMove(const OppData opp,const GameBoard& board) {
    auto [currentRow, currentCol] = player->getPos();
    auto [targetRow, targetCol] = opp.opponentPos;

    Direction currentDir = player->getDir();
    Direction desiredDir = calculateDirection(currentRow, currentCol, targetRow, targetCol);

    if(!player->isWaitingToReverse() && player->getWaitingToReverse())
         return Action::MoveBack;
    // Dodge incoming shells
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
    if (shouldShootOpponent(opp, board)&&  !player->isWaitingToShoot()) { //(currentDir == desiredDir
        return Action::Shoot;
    }
    if (currentDir != desiredDir) {
         Action rotate= determineRotation(currentDir, desiredDir);
         RotationOption option = rotationOption(rotate, desiredDir, currentDir ,board);
         if(option.canMove && option.safetyScore > 0)
         	return rotate;
    }

    // Move forward if not aligned/blocked
    pair<int,int> next = player->nextStep(true, board.getHeight(), board.getWidth());
    if ((currentRow != targetRow || currentCol != targetCol) && isOccupierFree(next, board)) {
        return Action::MoveFwd;
    }
    // Default fallback action
    return Action::Rotate1_8Right;
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
        {player->wrap(targetRow + dr, board.getHeight()), player->wrap(targetCol + dc, board.getWidth())}, // Next position
        {player->wrap(targetRow + dr*2, board.getHeight()), player->wrap(targetCol + dc*2, board.getWidth())} // Position after next
    };

    // Check if we're aligned with any predicted position
    for (const auto& [r, c] : predictedPositions) {
        if ((currentRow == r || currentCol == c) &&
            player->getDir() == calculateDirection(currentRow, currentCol, r, c)) {
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
            return a.canMove > b.canMove; // Prefer options that allow movement
        }
        return a.safetyScore > b.safetyScore;
    });

    // Return the best safe rotation
//    for (const auto& option : options) {
//        if (option.safetyScore > 0) {
//            return option.action;
//        }
//    }

    // If all options are bad, choose the least dangerous
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

//int WinAlgorithm::countOpenSpaceInDirection(Direction dir, const GameBoard& board, int steps) {
//    auto [row, col] = player->getPos();
//    auto [dr, dc] = offsets[static_cast<int>(dir)];
//
//    int openCount = 0;
//
//    for (int i = 1; i <= steps; i++) {
//        int newRow = player->wrap(row + dr * i, board.getHeight());
//        int newCol = player->wrap(col + dc * i, board.getWidth());
//
//        if (isOccupierFree({newRow, newCol}, board)){ //&&!willBeHitIn(newRow, newCol, i, board)) {
//            openCount++;
//        } else {
//            break; // Stop counting when we hit an obstacle
//        }
//    }
//
//    return openCount;
//}

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