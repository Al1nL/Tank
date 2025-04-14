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
    // move shells first
    for (auto& shell : player1->getFiredShells() ) {
      shell->move(*currGameState);
       // TODO: check how to do the go through wall and log a wall being gone (with logShellAction, sending relevant string)
    }
    for (auto& shell : player2->getFiredShells() ) { shell->move(*currGameState);
    //same here
    }

    // check if game ended after shell movement
    if (isGameOver()) {
        return;
    }
    //TODO: algo for moves
    Action action1 = player1->decideNextAction(*currGameState);
    Action action2 = player2->decideNextAction(*currGameState);

    // validate and execute
    if (validateAction(*player1, action1)) {
        applyAction(*player1,action1);
        logTankAction(*player1, action1, true);  // log good move for tank1
    } else {
        logTankAction(*player1, action1, false);  // log bad move for tank1
    }

    if (validateAction(*player2, action2)) {
        applyAction(*player2,action2);
        logTankAction(*player2, action2, true);  // log good move for tank2
    } else {
        logTankAction(*player2, action2, false);  // log bad move for tank2
    }
}

// TODO : Check for wall\Mine (IDK) for movefwd,vack in switch
bool GameManager::validateAction(Tank& tank, Action action) {
    // Only allow actions if the tank is not waiting for a backward move
    if (tank.isWaitingToReverse() && action != Action::MoveFwd)
        return false;
    if(tank.isWaitingToShoot() && action==Action::Shoot)
      return false;
    if(action == Action::Shoot)
        return validateShoot(tank);

    switch (action) {
        case Action::MoveFwd:
            //check for a wall? what else?
        case Action::MoveBack: //if not waiting for backward move - it's legal?
        case Action::Rotate1_8Left:
        case Action::Rotate1_4Left:
        case Action::Rotate1_8Right:
        case Action::Rotate1_4Right:
            return true;  // Rotation is always valid
        default:
            return false;
    }
}

bool GameManager::validateShoot(Tank& tank) { return !tank.isWaitingToShoot(); }

void GameManager::applyAction(Tank& tank, Action action) { tank.applyAction(action, *currGameState); }

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
void GameManager::logGameStart() {
  logs.push_back("Game Started!\n Player 1 | Position:" + player1->getPosition() + "\n Player 2 | Position:" + player2->getPosition());
}

void GameManager::logTankAction(const Tank& tank, Action action, bool success) {
    std::string result = success ? "Good move" : "Bad move";
    logs.push_back("Tank " + std::to_string(tank.getID()) + ": " + result + " - Action: " + actionToString(action) + " | Position: " + tank.getPosition());
}

void GameManager::logShellAction(Shell& shell, const std::string& whatHappened) {
    logs.push_back("Shell of Tank " + std::to_string(shell.getOwnerID()) + ": " + whatHappened + " | Position: " + shell.getPosition());
}

void GameManager::logWallWeakened(const pair<int, int>& pos) {
    logs.push_back("Wall Weakened | Position: [" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + "]");
}

void GameManager::logWallDestroyed(const pair<int, int>& pos) {
    logs.push_back("Wall Destroyed | Position: [" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + "]");
}

void GameManager::logShellsCollided(Shell& shell1, Shell& shell2) {
    logs.push_back("Shell Collision: Tank " + std::to_string(shell1.getOwnerID()) + " vs Tank " + std::to_string(shell2.getOwnerID()) + " | Position: " + shell1.getPosition());
}

void GameManager::logShellHitTank(Shell& shell, Tank& tank) {
    logs.push_back("Shell of Tank " + std::to_string(shell.getOwnerID()) + " hit Tank " + std::to_string(tank.getID()) + " | Position: " + shell.getPosition());
	if(tank.getID() == 1)
          p1Lost=true;
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
