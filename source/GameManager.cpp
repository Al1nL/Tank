#include "../headers/GameManager.h"
#include "../headers/Tank.h"
#include "../headers/GameBoard.h"

GameManager::GameManager(std::string filepath){
  currGameState = new GameBoard(filepath);
  player1 = new Tank(1, currGameState->getTankPosition(1)); //todo: check if pos -1-1 and make p1Lost true
  player2 = new Tank(2, currGameState->getTankPosition(2));
  logGameStart();
}

// TODO: runGameLoop, detectCollision, countDown for game(when both players are out of shells)-40 more steps till end of game
// TODO: all funcs on Tank and Shell, needs to update the shells for currentgamestate to be updated always
// TODO: isGameOver func, requestAction
void GameManager::processStep() {
  	pair<int, int> pos;

//    bool hitTank = false;
    // move shells first



    //moveFiredShells(*player1);
    //moveFiredShells(*player2);

    Action action1 = player1->decideNextAction(player2->getPos(), *currGameState);
    Action action2 = player2->decideNextAction(player1->getPos(), *currGameState);

    // validate and execute
    if(p1Lost || p2Lost) { return;}
    if (player1->isValidMove(*currGameState, action1)) {
        applyAction(*player1,action1);
    } else {
        logTankAction(*player1, action1, false);  // log bad move for tank1
    }

    if(p1Lost || p2Lost) { return;}
    if (player2->isValidMove(*currGameState, action2)) {
        applyAction(*player2,action2);
    } else {
        logTankAction(*player2, action2, false);  // log bad move for tank2
    }

    if(!isGameOver()) { moveFiredShells(); }
    if(!player1->getRemainingShells() && !player1->getRemainingShells()) stepsSinceNoShells++;
}
//
//// TODO : Already in Tank, not used here
//bool GameManager::validateAction(Tank tank, Action action) {
//    // Only allow actions if the tank is not waiting for a backward move
//    if (tank.isWaitingToReverse() && action != Action::MoveFwd)
//        return false;
//
//    pair<int,int> newPos;
//    OccupierType occupier;
//
//    switch (action) {
//        case Action::Shoot:
//          return !tank.isWaitingToShoot() && tank.getRemainingShells() > 0;
//        case Action::MoveFwd:
//        	newPos = tank.nextStep(true,currGameState->getHeight(),currGameState->getWidth());
//            occupier = currGameState->at(newPos).getOccupierType();
//			return occupier != OccupierType::Wall;
//        case Action::MoveBack:
//          	newPos = tank.nextStep(false, currGameState->getHeight(),currGameState->getWidth());
//            occupier = currGameState->at(newPos).getOccupierType();
//        	return !tank.isWaitingToReverse() && occupier != OccupierType::Wall;
//        case Action::Rotate1_8Left:
//        case Action::Rotate1_4Left:
//        case Action::Rotate1_8Right:
//        case Action::Rotate1_4Right:
//            return !tank.isWaitingToReverse();  // Rotation is always valid
//        default:
//            return false;
//    }
//}

//bool GameManager::validateShoot(Tank tank) { return !tank.isWaitingToShoot() && tank.getRemainingShells() > 0;}

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
           if (tank.getMovedBackwardLast() || (tank.getbBckwardCooldown() == 0 && tank.isWaitingToReverse())){
                // Instant backward move
               newPos=tank.nextStep(false,currGameState->getHeight(),currGameState->getWidth());
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
				tank.addShell(currGameState->getHeight(),currGameState->getWidth());
                tank.setShootCooldown(4);
                Shell* last = tank.getFiredShells().back();
                currGameState->updateFiredShells(last,true);
            }
            tank.setMovedBackwardLast(false);
            break;
    }
    int crash=0;
    if(action != Action::Shoot && tank.isWaitingToShoot())
       tank.setShootCooldown(tank.getShootCooldown() -1);
    if(newPos.first != -1){
      if(currGameState->at(newPos).getOccupierType() == OccupierType::Mine){
      	currGameState->updateBoard(newPos,newPos);
      	crash=1;
      }
      else if(currGameState->at(newPos).getOccupierType() == OccupierType::Tank){
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

void GameManager::countDown(){
	if (player1->getRemainingShells() == 0 || player2->getRemainingShells() == 0) {
          stepsSinceNoShells++;
	}
}

//
//void GameManager::goThroughCells(vector<Cell*> cells, Shell* shell) {
//  OccupierType occupier;
//  for (Cell* c : cells) {
//    occupier = c->getOccupierType();
//    if(c->hasShell()){
//      	Shell* s = c->getShell();
//      	s->getOwnerID() == 1 ? player1->deleteShell(c->getShell()) : player2->deleteShell(c->getShell());
//		shell->getOwnerID() ==1 ? player1->deleteShell(c->getShell()) : player2->deleteShell(c->getShell());
//        logShellsCollided(*shell, *c->getShell());
//        return;
//    }
//    else{
//      c->setShell(shell);
//      }
//    switch (occupier) {
//      case OccupierType::Tank:
//        c->getTank() == 1 ? logShellHitTank(*shell,*player1) : logShellHitTank(*shell,*player2);
//		return;
//      case OccupierType::Wall:
//        c->damageWall();
//        if(c->getOccupierType() == OccupierType::None){
//          logWallDestroyed(c->getPos());
//          return;
//        }
//        logWallWeakened(c->getPos());
//        break;
//
//    }
//  }
//}


//vector<Shell*> GameManager::getAllFiredShells() {
//    vector<Shell*> allShells;
//    const auto &f1 = player1->getFiredShells();
//    const auto &f2 = player2->getFiredShells();
//    if (!f1.empty())
//        allShells.insert(allShells.end(), f1.begin(), f1.end());
//    if (!f2.empty())
//        allShells.insert(allShells.end(), f2.begin(), f2.end());
//
//    return allShells;
//}

void GameManager::updateShellPositions(vector<Shell*>& allShells, map<Shell*, pair<int, int>>& previousPositions, map<pair<int, int>, vector<Shell*>>& cellToShells) {
    for (Shell* shell : allShells) {
        // Store the previous position
        auto oldPos = shell->getPos();
        previousPositions[shell] = oldPos;
        Cell& c=currGameState->at(oldPos);
        if(c.getOccupierType() == OccupierType::Tank){
            currGameState->at(oldPos).getTank() == 1 ? logShellHitTank(*shell, *player1) : logShellHitTank(*shell, *player2);
            break;
        }
        if(!isGameOver()){
            //check if the shell is newly created in a cell with a wall
            if(c.getOccupierType() == OccupierType::Wall&& shell->isNewShell())
            {
                c.damageWall();
                if (c.getOccupierType() == OccupierType::None)
                    logWallDestroyed(oldPos);
                else
                    logWallWeakened(oldPos);
            }
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
        }
    }
}

void GameManager::removeShellFromGame(Shell* shell, vector<Shell*>& allShells) {
    if (shell) {
      // Deletes the shell from the vector
        allShells.erase(remove(allShells.begin(), allShells.end(), shell), allShells.end());
        currGameState->updateFiredShells(shell,false);
        shell->getOwnerID() == 1 ? player1->deleteShell(shell) : player2->deleteShell(shell);
    }
}
void GameManager::handleShellCollision(vector<Shell*>& allShells, map<Shell*,pair<int,int>>& previousPositions,int step,map<pair<int, int>, vector<Shell*>> &cellToShells) {

    for (auto& [shell, prevPos] : previousPositions) {
        // Handle 2 shells headed toward eachother ><
        for (auto& [otherShell, otherPrevPos] : previousPositions) {
            if (otherShell != shell && otherPrevPos == shell->getPos() && prevPos == otherShell->getPos()) {
              logShellsCollided(*shell, *otherShell);
              removeShellFromGame(otherShell, allShells);
              removeShellFromGame(shell, allShells);
                // Log the collision between the two shells
                std::cerr << "[DEBUG] head-on collision between shells at "
                            << shell->getPos().first<<","<<shell->getPos().second<<"\n";
            }
        }
    }
    // Handle shells at the same cell
    for (auto& [pos, shells] : cellToShells) {
        Cell& cell = currGameState->at(pos);
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

        if (validShells.size() > 1) {
            std::cerr<<"[DEBUG] multi-shell collision in cell\n";

            // All valid shells in the same cell will be destroyed
            logShellsCollided(*validShells[0], *validShells[1]);  // Log collision between shells
            for (Shell* s : validShells) {
                removeShellFromGame(s, allShells);
            continue;
            }
        }
        // Handle other collisions
        if (!validShells.empty()) {
            Shell* shell = validShells[0];
            OccupierType occ = cell.getOccupierType();
            pair<int,int> oldPos = shell->getPos();

            switch (occ) {
                case OccupierType::Tank:
                    cell.getTank() == 1 ? logShellHitTank(*shell, *player1) : logShellHitTank(*shell, *player2);
                    removeShellFromGame(shell, allShells);
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
            else{
                // Update the board with the new position of the shell
                currGameState->updateBoard(oldPos, pos);
                shell->setPos(pos);
            }


        }
    }
}
void GameManager::moveFiredShells() {

    vector<Shell*> allShells = currGameState->getAllFiredShells(); //why not get fom tanks - this will requrie additional handaling
    // Create a map to store previous positions of shells
    map<Shell*, pair<int, int>> previousPositions;

    for (int step = 0; step < 2; step++) {
        map<pair<int, int>, vector<Shell*>> cellToShells;
        // Update the position of all shells
        updateShellPositions(allShells, previousPositions, cellToShells);
        // Handle all possible collisions
        if(isGameOver()) break;
        handleShellCollision(allShells, previousPositions, step, cellToShells);
    }
}

//void GameManager::moveFiredShells(Tank& tank) {
//    vector<Cell*> cell;
//    for(int step=0;step<2;step++){
//        for (auto& shell : tank.getFiredShells() ) {
//           //      pos=shell->nextStep(true,currGameState->getHeight());
//            cell = currGameState->getCells(shell->getPos(), shell->getOffset(),1);// shell->getSteps());
//            goThroughCells(cell,shell);
//            if(isGameOver()) {
//                return;
//            }
//            currGameState->updateBoard( shell->getPos(), cell[shell->getSteps()-1]->getPos());
//            shell->setPos(cell[shell->getSteps()-1]->getPos());
//        }
//    }
//}

bool GameManager::isGameOver(){
	if( p1Lost || p2Lost || stepsSinceNoShells == 40) return true;

    return false;
}

void GameManager::endGame() {
   p1Lost && !p2Lost ? logGameOver(2) : !p1Lost && p2Lost ? logGameOver(1) : logGameOver(-1);
   delete player1;
   delete player2;
   delete currGameState;
   writeOutput();
}

void GameManager::printCurrentState() {
  currGameState->printBoard();
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

string GameManager::directionToString(Direction dir) {
    switch (dir) {
        case U:  return "U";
        case UR: return "UR";
        case R:  return "R";
        case DR: return "DR";
        case D:  return "D";
        case DL: return "DL";
        case L:  return "L";
        case UL: return "UL";
        default: return "Unknown";
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
    string result = success ? "Good move" : "Bad move",log;
    if(action == Action::MoveFwd||action == Action::MoveBack) {
      log = "Tank " + to_string(tank.getID()) + ": " + result + " - Action: " + actionToString(action)+ " in direction "+ directionToString(tank.getDir()) + " | Updated Position: " + tank.getPosition();
    }
    else
      {
        log = "Tank " + to_string(tank.getID()) + ": " + result + " - Action: " + actionToString(action)+ " in direction "+ directionToString(tank.getDir()) + " | Position: " + tank.getPosition();
      }
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logShellMove(Shell& shell, pair<int,int> newPos) {
   string log ="Shell of Tank " +to_string(shell.getOwnerID()) + " is Moving in direction: " + directionToString(shell.getDir()) + " | Updated Position: [" + to_string(newPos.first)+", "+to_string(newPos.second)+"]";
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logWallWeakened(const pair<int, int>& pos) {
  string log ="Wall Weakened | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logWallDestroyed(const pair<int, int>& pos) {
  string log = "Wall Destroyed | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logTankOnMine(Tank& tank, const pair<int, int>& pos){
  string log = "Tank " + to_string(tank.getID()) + " stepped on mine | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
  tank.getID() == 1 ? p1Lost =true: p2Lost = true;
    currGameState->at(tank.getPos()).destroyOccupier();

    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logTankOnTank(Tank& tank, const pair<int, int>& pos) {
  string log = "Tank " + to_string(tank.getID()) + " collided with another Tank | Position: [" + to_string(pos.first) + ", " + to_string(pos.second) + "]";
  p1Lost =true;
  p2Lost = true;
    currGameState->at(tank.getPos()).destroyOccupier();

    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logShellsCollided(Shell& shell1, Shell& shell2) {  //todo: change?? case where more than 2 collided what to do?
  string log = "Shell Collision: Tank " + to_string(shell1.getOwnerID()) + " vs Tank " + to_string(shell2.getOwnerID()) + " | Position: " + shell1.getPosition();
    logs.push_back(log);
    cerr << log << endl;
}

void GameManager::logShellHitTank(Shell& shell, Tank& tank) {
  string log = "Shell of Tank " + to_string(shell.getOwnerID()) + " hit Tank " + to_string(tank.getID()) + " | Position: " + tank.getPosition();
    logs.push_back(log);
	tank.getID() == 1 ? p1Lost=true : p2Lost =true;
    currGameState->at(tank.getPos()).destroyOccupier();
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
