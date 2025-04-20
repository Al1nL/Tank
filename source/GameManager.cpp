#include "../headers/GameManager.h"
#include "../headers/Tank.h"
#include "../headers/GameBoard.h"

GameManager::GameManager(std::string filepath){
  currGameState = new GameBoard(filepath, *this);
  player1 = new Tank(1, currGameState->getTankPosition(1), *this);
  player2 = new Tank(2, currGameState->getTankPosition(2), *this);
  logGameStart();
}

// TODO: runGameLoop, detectCollision, countDown for game(when both players are out of shells)-40 more steps till end of game
// TODO: all funcs on Tank and Shell, needs to update the shells for currentgamestate to be updated always
// TODO: isGameOver func, requestAction
void GameManager::processStep() {
  	pair<int, int> pos;
    vector<Cell*> cells;
    bool hitTank = false;
    // move shells first
    for (auto& shell : player1->getFiredShells() ) {
//      pos=shell->nextStep(true,currGameState->getHeight());
		cells = currGameState->getCells(shell->getPos(), shell->getOffset(), shell->getSteps());
		goThroughCells(cells,shell);
        if(isGameOver()) {
          return;
        }
    }
    for (auto& shell : player2->getFiredShells() ) {
      	cells = currGameState->getCells(shell->getPos(), shell->getOffset(), shell->getSteps());
		goThroughCells(cells,shell);
    	if(isGameOver()) {
      		return;
    	}
    }


    //TODO: algo for moves
    Action action1 = player1->decideNextAction(player2->getPos(), *currGameState);
    Action action2 = player2->decideNextAction(player1->getPos(), *currGameState);

    // validate and execute
    if (player1->isValidMove(*currGameState, action1)) {
        applyAction(*player1,action1);
        logTankAction(*player1, action1, true);  // log good move for tank1
    } else {
        logTankAction(*player1, action1, false);  // log bad move for tank1
    }

    if (player2->isValidMove(*currGameState, action2)) {
        applyAction(*player2,action2);
        logTankAction(*player2, action2, true);  // log good move for tank2
    } else {
        logTankAction(*player2, action2, false);  // log bad move for tank2
    }

    if(!player1->getRemainingShells() && !player1->getRemainingShells()) stepsSinceNoShells++;
}

// TODO : Check for wall\Mine (IDK) for movefwd,vack in switch
bool GameManager::validateAction(Tank tank, Action action) {
    // Only allow actions if the tank is not waiting for a backward move
    if (tank.isWaitingToReverse() && action != Action::MoveFwd)
        return false;

    pair<int,int> newPos;
    OccupierType occupier;

    switch (action) {
        case Action::Shoot:
          return !tank.isWaitingToShoot() && tank.getRemainingShells() > 0;
        case Action::MoveFwd:
            //check for a wall? what else?
        	newPos = tank.nextStep(true,currGameState->getHeight());
            if(newPos.second >=  currGameState->getWidth() || newPos.second < 0) return false;
            occupier = currGameState->at(newPos).getOccupierType();
			return occupier != OccupierType::Tank && occupier != OccupierType::Wall;
        case Action::MoveBack: //if not waiting for backward move - it's legal?
          	newPos = tank.nextStep(false, currGameState->getHeight());
           	if(newPos.second >=  currGameState->getWidth() || newPos.second < 0) return false;
            occupier = currGameState->at(newPos).getOccupierType();
        	return !tank.isWaitingToReverse() && occupier != OccupierType::Tank && occupier != OccupierType::Wall;
        case Action::Rotate1_8Left:
        case Action::Rotate1_4Left:
        case Action::Rotate1_8Right:
        case Action::Rotate1_4Right:
            return !tank.isWaitingToReverse();  // Rotation is always valid
        default:
            return false;
    }
}

bool GameManager::validateShoot(Tank tank) { return  !tank.isWaitingToShoot() && tank.getRemainingShells() > 0;}

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
                newPos = tank.nextStep(true,currGameState->getHeight());
            }
            tank.setMovedBackwardLast(false);
            break;
        case Action::MoveBack:
           if (tank.getMovedBackwardLast() || (tank.getbBckwardCooldown() == 0 && tank.isWaitingToReverse())){
                // Instant backward move
                newPos = tank.nextStep(false,currGameState->getHeight());
                tank.setWaitingForBackward(false);
            }
           else if (tank.getbBckwardCooldown() == 0) {
                // Start the backward move cooldown (waiting for 2 steps)
                tank.setBackwardCooldown(2);
                tank.setWaitingForBackward(true);
            }
            tank.setMovedBackwardLast(false);
            break;
        case Action::Rotate1_8Left:
        case Action::Rotate1_8Right:
        case Action::Rotate1_4Left:
        case Action::Rotate1_4Right:
//            movedBackwardLast = false;
            tank.rotate(action);
            tank.setMovedBackwardLast(false);
            break;
        case Action::Shoot:
            if (!tank.isWaitingToShoot()) {
				tank.addShell();
                tank.setShootCooldown(4);
            }
            tank.setMovedBackwardLast(false);
            break;
    }
    if(action != Action::Shoot && tank.isWaitingToShoot())
       tank.setShootCooldown(tank.getShootCooldown() -1);

    if(newPos.first != -1){
      currGameState->updateBoard(tank.getPos(),newPos);
      tank.setPos(newPos);
    }
}

void GameManager::countDown(){
	if (player1->getRemainingShells() == 0 || player2->getRemainingShells() == 0) {
          stepsSinceNoShells++;
	}
}

//Log Functions

string GameManager::actionToString(Action action) {
    switch (action) {
        case Action::MoveFwd: return "Move Forward";
        case Action::MoveBack: return "Move Backward";
        case Action::Rotate1_8Left: return "Rotate 1/8 Left";
        case Action::Rotate1_4Left: return "Rotate 1/4 Left";
        case Action::Rotate1_8Right: return "Rotate 1/8 Right";
        case Action::Rotate1_4Right: return "Rotate 1/4 Right";
        case Action::Shoot: return "Shoot";
        default: return "Unknown Action";
    }
}

void GameManager::goThroughCells(vector<Cell*> cells, Shell* shell) {
  OccupierType occupier;
  for (Cell* c : cells) {
    occupier = c->getOccupierType();
    if(c->hasShell()) {
      	c->getShell()->getOwnerID() == 1 ? player1->deleteShell(c->getShell()) : player2->deleteShell(c->getShell());
		shell->getOwnerID() ==1 ? player1->deleteShell(c->getShell()) : player2->deleteShell(c->getShell());
    }
    switch (occupier) {
      case OccupierType::Tank:
        c->getTank() == 1 ? logShellHitTank(*shell,*player1) : logShellHitTank(*shell,*player2);

        break;
      case OccupierType::Wall:
        c->damageWall();
         break;

    }
  }
}

void GameManager::logGameStart() {
  string log = "Game Started!\n Player 1 | Position:" + player1->getPosition() + "\n Player 2 | Position:" + player2->getPosition();
  logs.push_back(log);
  cerr << log << endl;
}

void GameManager::logGameOver(int winner) {
  string win = (winner == -1) ? "End in tie" :  "The winner is: " + to_string(winner);
  logs.push_back("Game Over!\n" + win);
}

void GameManager::logTankAction(const Tank& tank, Action action, bool success) {
    std::string result = success ? "Good move" : "Bad move";
    string log = "Tank " + std::to_string(tank.getID()) + ": " + result + " - Action: " + actionToString(action) + " | Position: " + tank.getPosition();
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logShellAction(Shell& shell, const std::string& whatHappened) {
   string log ="Shell of Tank " + std::to_string(shell.getOwnerID()) + ": " + whatHappened + " | Position: " + shell.getPosition();
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logWallWeakened(const pair<int, int>& pos) {
  string log ="Wall Weakened | Position: [" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + "]";
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logWallDestroyed(const pair<int, int>& pos) {
  string log = "Wall Destroyed | Position: [" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + "]";
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logShellsCollided(Shell& shell1, Shell& shell2) {
  string log = "Shell Collision: Tank " + std::to_string(shell1.getOwnerID()) + " vs Tank " + std::to_string(shell2.getOwnerID()) + " | Position: " + shell1.getPosition();
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logShellHitTank(Shell& shell, Tank& tank) {
  string log = "Shell of Tank " + std::to_string(shell.getOwnerID()) + " hit Tank " + std::to_string(tank.getID()) + " | Position: " + shell.getPosition();
    logs.push_back(log);
	tank.getID() == 1 ? p1Lost=true : p2Lost =true;
    cerr << log << endl;
}
void GameManager::writeOutput() {
    logFile.open("GameLog.txt", std::ios::out | std::ios::trunc);  // Open file in write mode and truncate it
    // Write the logs to the file
    for (const auto& entry : logs) {
        logFile << entry << "\n";
    }

    logFile.close();  // Close the file
    logs.clear();  // Clear the log entries
}

bool GameManager::isGameOver(){
	if( p1Lost || p2Lost || stepsSinceNoShells == 40) return true;

    return false;
}

void GameManager::endGame() {
   p1Lost ? logGameOver(2) : p2Lost ? logGameOver(1) : logGameOver(-1);
   delete player1;
   delete player2;
   delete currGameState;
   writeOutput();
}

void GameManager::printCurrentState() {
  currGameState->printBoard();
}