#include "../headers/GameManager.h"
#include "../headers/Tank.h"
#include "../headers/GameBoard.h"

/**
 * @brief Constructor.
 *
 * Initializes the board and tanks.
 * @param filepath Path to the file describing the initial board state.
 */
GameManager::GameManager(std::string filepath) {
	currGameState = new GameBoard(filepath);
	player1 = new Tank(1, currGameState->getTankPosition(1));
	player2 = new Tank(2, currGameState->getTankPosition(2));
	logGameStart();
}

/**
 * @brief Processes a single step of the game: moves shells, tanks, and handles actions and collisions.
 */
void GameManager::processStep() {
	pair<int, int> pos;
	moveFiredShells();

	if(isGameOver()) {
		return;
	}

	Action action1 = player1->decideNextAction({player2->getPos(),player2->getDir()}, *currGameState);
	Action action2 = player2->decideNextAction({player1->getPos(),player1->getDir()}, *currGameState);

	// validate and execute
	if (player1->isValidMove(*currGameState, action1)) {
		applyAction(*player1,action1);
	} else {
		logTankAction(*player1, action1, false);  // log bad move
	}

	if(isGameOver()) {
		return;
	}
	if (player2->isValidMove(*currGameState, action2)) {
		applyAction(*player2,action2);
	}
	else {
		logTankAction(*player2, action2, false);
	}

	// Updating cool\countdowns
	if(!player1->getRemainingShells() && !player1->getRemainingShells()) {
		stepsSinceNoShells++;
	}
	if(player1->getBackwardCooldown() > 0 && action1 != Action::MoveBack) {
		player1->setBackwardCooldown(player1->getBackwardCooldown()-1);
	}
	if(player2->getBackwardCooldown() > 0 && action2 != Action::MoveBack) {
		player2->setBackwardCooldown(player2->getBackwardCooldown()-1);
	}
}

/**
 * @brief Applies the given action to the specified tank (assumes the action is valid).
 *
 * @param tank Reference to the tank to apply the action on.
 * @param action The action to apply.
 */
void GameManager::applyAction(Tank& tank, Action action) {
  	pair<int,int> newPos = {-1,-1};
  //assuming its valid move
    switch(action){
        case Action::MoveFwd:
            if (tank.isWaitingToReverse()) {
                tank.setWaitingForBackward(false);
                tank.setBackwardCooldown(0);
                    // Tank stays in place! No forward move happens!
            } else {
              newPos=tank.nextStep(true,currGameState->getHeight(),currGameState->getWidth());
            }
            tank.setMovedBackwardLast(false);
            break;
        case Action::MoveBack:
           if (tank.getMovedBackwardLast() || (tank.getBackwardCooldown() == 0 && tank.getWaitingToReverse())){
                // Instant backward move
               newPos=tank.nextStep(false,currGameState->getHeight(),currGameState->getWidth());
                tank.setWaitingForBackward(false);
                tank.setMovedBackwardLast(true);
            }
           else if (tank.getBackwardCooldown() == 0) {
                // Start the backward move cooldown (waiting for 2 steps)
                tank.setBackwardCooldown(2);
                tank.setWaitingForBackward(true);
            }

		break;
	case Action::Rotate1_8Left:
	case Action::Rotate1_8Right:
	case Action::Rotate1_4Left:
	case Action::Rotate1_4Right:
		tank.rotate(action);
		tank.setMovedBackwardLast(false);
		break;
	case Action::Shoot:
		if (!tank.isWaitingToShoot()) {
			tank.addShell(currGameState->getHeight(),currGameState->getWidth());
			tank.setShootCooldown(4);
			Shell* last = tank.getFiredShells().back();
			currGameState->updateFiredShells(last,true);
			const_cast<Cell&>(currGameState->at(last->getPos())).setShell(last);
		}
		tank.setMovedBackwardLast(false);
		break;
	}

	int crash=0;

	if(action != Action::Shoot && tank.isWaitingToShoot())
		tank.setShootCooldown(tank.getShootCooldown() -1);
	if(newPos.first != -1) {
		if(currGameState->at(newPos).getOccupierType() == OccupierType::Mine) {
			currGameState->updateBoard(newPos,newPos);
			crash=1;
		}
		else if(currGameState->at(newPos).getOccupierType() == OccupierType::Tank) {
			crash=2;
		}
		currGameState->updateBoard(tank.getPos(),newPos);
		tank.setPos(newPos);
	}
	logTankAction(tank, action, true);
	if(crash==1)
		logTankOnMine(tank, newPos);
	else if(crash==2)
		logTankOnTank(tank, newPos);

	return;
}

/**
 * @brief Increments the counter when no shells are left.
 */
void GameManager::countDown() {
	if (player1->getRemainingShells() == 0 || player2->getRemainingShells() == 0) {
		stepsSinceNoShells++;
	}
}

/**
 * @brief Updates all shell positions, handling wall collisions if necessary.
 *
 * @param allShells Vector of all active shells.
 * @param previousPositions Map storing each shell's previous position.
 * @param cellToShells Map from a cell position to the list of shells that moved there.
 */
void GameManager::updateShellPositions(vector<Shell*>& allShells, map<Shell*, pair<int, int>>& previousPositions, map<pair<int, int>, vector<Shell*>>& cellToShells) {
	for (Shell* shell : allShells) {
		// Store the previous position
		auto oldPos = shell->getPos();
		previousPositions[shell] = oldPos;
		Cell& c=const_cast<Cell&>(currGameState->at(oldPos));
		if(c.getOccupierType() == OccupierType::Tank) {
			c.getTank() == 1 ? logShellHitTank(*shell, *player1) : logShellHitTank(*shell, *player2);
			break;
		}
		if(!isGameOver()) {
			//check if the shell is newly created in a cell with a wall
			if(c.getOccupierType() == OccupierType::Wall&& shell->isNewShell())
				handleWallCollision(c);
			if(shell->isNewShell())
				shell->setNotNewShell();
			// Determine the next position for the shell
			vector<Cell*> path = currGameState->getCells(shell->getPos(), shell->getOffset(), 1);
			Cell* nextCell = path[0];
			auto p = nextCell->getPos();

			logShellMove(*shell, p);

             // Store the shell in the new cell
            if(cellToShells.count(p) == 0)
          	{
            	cellToShells.insert({p,{}});
          	}
             cellToShells[p].push_back(shell);
             c.setShell(nullptr);
             currGameState->updateBoard(oldPos, p);
             shell->setPos(p);
        }
    }
}
/**
 * @brief Removes a shell from the board and game systems.
 *
 * @param shell Pointer to the shell to remove.
 * @param allShells Vector of all active shells.
 * @param cellToShells Map from a cell position to the list of shells.
 */
void GameManager::removeShellFromGame(Shell* shell, vector<Shell*>& allShells, map<pair<int, int>, vector<Shell*>> &cellToShells) {
    if (shell) {
      // Deletes the shell from the vector
      	pair<int, int> pos = shell->getPos();
        if (cellToShells.count(pos)) {
            auto& shellsInCell = cellToShells[pos];
            shellsInCell.erase(remove(shellsInCell.begin(), shellsInCell.end(), shell), shellsInCell.end());
        }
        allShells.erase(remove(allShells.begin(), allShells.end(), shell), allShells.end());
        currGameState->updateFiredShells(shell,false);
        shell->getOwnerID() == 1 ? player1->deleteShell(shell) : player2->deleteShell(shell);
    }
}

/**
 * @brief Handles collisions between shells and between shells and other objects.
 *
 * @param allShells Vector of all active shells.
 * @param previousPositions Map storing each shell's previous position.
 * @param step Current shell movement step (0 or 1).
 * @param cellToShells Map from a cell position to the list of shells.
 */
void GameManager::handleShellCollision(vector<Shell*>& allShells, map<Shell*,pair<int,int>>& previousPositions,int step,map<pair<int, int>, vector<Shell*>> &cellToShells) {
	// Identify all collisions

	// Identifies all head collision >< (2 shells passed each other)
	set<pair<Shell*, Shell*>> collisions;
	for (auto& [shell, prevPos] : previousPositions) {
    	for (auto& [otherShell, otherPrevPos] : previousPositions) {
        	if (otherShell == shell) continue;

            // Check if shells are moving towards each other (head-on collision)
            bool isHeadOn = (prevPos == otherShell->getPos() && otherPrevPos == shell->getPos());

            // Also check if shells are now in the same position (direct hit)
            bool isSamePos = (shell->getPos() == otherShell->getPos());

            if ((isHeadOn || isSamePos) &&
                collisions.find({shell, otherShell}) == collisions.end() &&
                collisions.find({otherShell, shell}) == collisions.end()) {
                collisions.insert({shell, otherShell});
            }
    	}
	}
	// Remove from game systems to avoid looking at destroyed shells at the resst of the checks
	for (auto& [shell1, shell2] : collisions) {
		logShellsCollided({shell1, shell2});

		removeShellFromGame(shell1, allShells, cellToShells);
		removeShellFromGame(shell2, allShells, cellToShells);
	}

    // Handle shells at the same cell
    for (auto& [pos, shells] : cellToShells) {
      	bool collided = false;
      	if(shells.empty()) continue;
        Cell& cell = const_cast<Cell&>(currGameState->at(pos));
        vector<Shell*> validShells;
        if (shells.size() > 1) {
            // Multiple shells go to same cell: check if the shells are in the remaining allShells
            for (Shell* s : shells) {
                // Only add shells that are still valid (not destroyed)
                if (find(allShells.begin(), allShells.end(), s) != allShells.end()) {
                    validShells.push_back(s);
                }
            }
        }
        else //only 1 shell in this cell
            validShells.push_back(shells[0]);

        Shell* shell = validShells[0];
		OccupierType occ = cell.getOccupierType();
        if (validShells.size() > 1) {
            // All valid shells in the same cell will be destroyed
            collided = true;
            logShellsCollided(validShells);  // Log collision between shells besides one to handle situation of other objects in the cell
            if(occ == OccupierType::Wall)
				handleWallCollision(cell);
        }

        for (size_t i = 1; i < validShells.size(); ++i) {
            removeShellFromGame(validShells[i], allShells, cellToShells);
        }

        switch (occ) {
            case OccupierType::Tank:
                cell.getTank() == 1 ? logShellHitTank(*shell, *player1) : logShellHitTank(*shell, *player2);
                removeShellFromGame(shell, allShells,cellToShells);
                break;
            case OccupierType::Wall:
                // If the cell contains a wall, damage it
                cell.damageWall();
                if (cell.getOccupierType() == OccupierType::None)
                    logWallDestroyed(pos);
                else
                    logWallWeakened(pos);
                break;
        }
        if (find(allShells.begin(), allShells.end(), shell) == allShells.end())
          cell.setShell(nullptr);
        else if(collided){ removeShellFromGame(shell, allShells,cellToShells);}
//        else{
//            // Update the board with the new position of the shell
//            currGameState->updateBoard(oldPos, pos);
//
//        }
    }
}
/**
 * @brief Moves all fired shells two steps forward, handling their movement and collisions.
 */
void GameManager::moveFiredShells() {
	vector<Shell*> allShells = currGameState->getAllFiredShells();
	// Create a map to store previous positions of shells

	for (int step = 0; step < 2; step++) {
		map<Shell*, pair<int, int>> previousPositions;
		map<pair<int, int>, vector<Shell*>> cellToShells;
		// Update the position of all shells
		updateShellPositions(allShells, previousPositions, cellToShells);
		// Handle all possible collisions
		if(isGameOver()) break;
		handleShellCollision(allShells, previousPositions, step, cellToShells);
	}
}

/**
 * @brief Checks if the game has ended due to tank loss or inactivity.
 *
 * @return True if the game is over, false otherwise.
 */
bool GameManager::isGameOver() {
	if( p1Lost || p2Lost || stepsSinceNoShells == 40) return true;
	return false;
}

/**
 * @brief Finalizes the game, determines the winner, frees resources, and logs the result.
 */
void GameManager::endGame() {
	p1Lost && !p2Lost ? logGameOver(2) : !p1Lost && p2Lost ? logGameOver(1) : logGameOver(-1);
	delete player1;
	delete player2;
	delete currGameState;
	writeOutput();
}

/**
 * @brief Prints the current state of the board.
 */
void GameManager::printCurrentState() {
	currGameState->printBoard();
}
// ------------------ Logging Functions ------------------

/**
 * @brief Converts an Action enum value to its corresponding string representation.
 *
 * @param action The action to convert.
 * @return String representation of the action.
 */
string GameManager::actionToString(Action action) {
	switch (action) {
	case Action::MoveFwd:
		return "Move Forward";
	case Action::MoveBack:
		return "Move Backward";
	case Action::Rotate1_8Left:
		return "Rotate 1/8 Left";
	case Action::Rotate1_4Left:
		return "Rotate 1/4 Left";
	case Action::Rotate1_8Right:
		return "Rotate 1/8 Right";
	case Action::Rotate1_4Right:
		return "Rotate 1/4 Right";
	case Action::Shoot:
		return "Shoot";
	default:
		return "Unknown Action";
	}
}

/**
 * @brief Converts a Direction enum value to its corresponding string representation.
 *
 * @param dir The direction to convert.
 * @return String representation of the direction.
 */
string GameManager::directionToString(Direction dir) {
	switch (dir) {
	case U:
		return "U";
	case UR:
		return "UR";
	case R:
		return "R";
	case DR:
		return "DR";
	case D:
		return "D";
	case DL:
		return "DL";
	case L:
		return "L";
	case UL:
		return "UL";
	default:
		return "Unknown";
	}
}

/**
 * @brief Logs the start of the game with tank initial positions.
 */
void GameManager::logGameStart() {
	string log = "Game Started!\n Player 1 | Position:" + player1->getPosition() + "\n Player 2 | Position:" + player2->getPosition();
	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs the end of the game and the winner (or tie).
 * @param winner Tank ID of the winner (or -1 if tie).
 */
void GameManager::logGameOver(int winner) {
	string win = (winner == -1) ? "End in tie" :  "The winner is: " + to_string(winner);
	logs.push_back("Game Over!\n" + win);
}

/**
 * @brief Logs a tank's attempted action and its validity - bad or good.
 *
 * @param tank The tank performing the action.
 * @param action The action attempted.
 * @param success Whether the action was successful.
 */
void GameManager::logTankAction(const Tank& tank, Action action, bool success) {
	string result = success ? "Good move" : "Bad move",log;
	if(action == Action::MoveFwd||action == Action::MoveBack) {
		log = "Tank " + to_string(tank.getID()) + ": " + result + " - Action: " + actionToString(action)+ " facing direction "+ directionToString(tank.getDir()) + " | Updated Position: " + tank.getPosition();
	}
	else
	{
		log = "Tank " + to_string(tank.getID()) + ": " + result + " - Action: " + actionToString(action)+ " facing direction "+ directionToString(tank.getDir()) + " | Position: " + tank.getPosition();
	}
	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs the movement of a shell to a new position.
 *
 * @param shell The shell being moved.
 * @param newPos The new position.
 */
void GameManager::logShellMove(Shell& shell, pair<int,int> newPos) {
	string log ="Shell of Tank " +to_string(shell.getOwnerID()) + " is Moving in direction: " + directionToString(shell.getDir()) + " | Updated Position: [" + to_string(newPos.first)+", "+to_string(newPos.second)+"]";
	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs that a wall has been weakened.
 *
 * @param pos Position of the weakened wall.
 */
void GameManager::logWallWeakened(const pair<int, int>& pos) {
	string log ="Wall Weakened | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs that a wall has been destroyed.
 *
 * @param pos Position of the destroyed wall.
 */
void GameManager::logWallDestroyed(const pair<int, int>& pos) {
	string log = "Wall Destroyed | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs that a wall has been destroyed.
 *
 * @param tank Reference to the tank that stepped on a mine
 * @param pos Position of the collision
 */
void GameManager::logTankOnMine(Tank& tank, const pair<int, int>& pos) {
	string log = "Tank " + to_string(tank.getID()) + " stepped on mine | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
	tank.getID() == 1 ? p1Lost =true: p2Lost = true;
	const_cast<Cell&>(currGameState->at(tank.getPos())).destroyOccupier();

	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs that a wall has been destroyed.
 *
 * @param tank Reference to the tank that stepped on another tank
 * @param pos Position of the collision.
 */
void GameManager::logTankOnTank(Tank& tank, const pair<int, int>& pos) {
	string log = "Tank " + to_string(tank.getID()) + " collided with another Tank | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
	p1Lost =true;
	p2Lost = true;
	const_cast<Cell&>(currGameState->at(tank.getPos())).destroyOccupier();

	logs.push_back(log);
	cerr << log << endl;
}
/**
 * @brief Logs that a wall has been destroyed.
 *
 * @param shells Vector of pointers to all the shells that collided
 * @param pos Position of the collision.
 */
void GameManager::logShellsCollided(vector<Shell*> shells) {  //todo: change?? case where more than 2 collided what to do?
	string log = "Shell Collision: Number of shells: "+to_string(shells.size())+"| belong to tank: ";
	vector<int> ids;
	for(auto shell : shells) {
		if(find(ids.begin(), ids.end(),shell->getOwnerID()) == ids.end()) {
			log+= to_string(shell->getOwnerID()) +" ";
			ids.push_back(shell->getOwnerID());
		}
	}
	log += "| Position: " + shells[0]->getPosition();
	logs.push_back(log);
	cerr << log << endl;
}

/**
 * @brief Logs that a wall has been destroyed.
 *
 * @param shell Reference to a shell that hit a tank
 * @tank The tank that's been hit.
 */
void GameManager::logShellHitTank(Shell& shell, Tank& tank) {
	string log = "Shell of Tank " + to_string(shell.getOwnerID()) + " hit Tank " + to_string(tank.getID()) + " | Position: " + tank.getPosition();
	logs.push_back(log);
	tank.getID() == 1 ? p1Lost=true : p2Lost =true;
	const_cast<Cell&>(currGameState->at(tank.getPos())).destroyOccupier();
	cerr << log << endl;
}

/**
 * @brief Writes all logs to 'GameLog.txt' file
 */
void GameManager::writeOutput() {
	logFile.open("GameLog.txt", std::ios::out | std::ios::trunc);  // Open file in write mode and truncate it
	// Write the logs to the file
	for (const auto& entry : logs) {
		logFile << entry << "\n";
	}

	logFile.close();  // Close the file
	logs.clear();  // Clear the log entries
}