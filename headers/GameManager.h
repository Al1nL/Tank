#ifndef GAMEMANAGER_H  // Header guard to prevent multiple inclusions
#define GAMEMANAGER_H
#include <vector>
#include <map>
#include <set>
#include <algorithm> //for find func
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include "Action.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include "Cell.h"
#include "OppData.h"

// Forward declarations
class Tank;
class GameBoard;
class Shell;
class Cell;
struct OppData;
using namespace std;

class GameManager{
    GameBoard* currGameState;
    Tank* player1;
    Tank* player2;
    int stepCounter=0;
    int stepsSinceNoShells = 0;
    vector<string> logs;
    ofstream logFile;

    bool p1Lost=false;
    bool p2Lost=false;

    // Shell movement helpers
    void moveFiredShells();
    void removeShellFromGame(Shell* shell, vector<Shell*>& allShells,map<pair<int, int>, vector<Shell*>> &cellToShells);
    void handleShellCollision(vector<Shell*>& allShells, map<Shell*,pair<int,int>>& previousPositions,int step,map<pair<int, int>, vector<Shell*>> &cellToShells);
    void updateShellPositions(vector<Shell*>& allShells, map<Shell*, pair<int, int>>& previousPositions, map<pair<int, int>, vector<Shell*>>& cellToShells);
    const vector<Shell*> getAllFiredShells();

  public:
    GameManager();
    GameManager(string filepath);
    ~GameManager(){}

    // Game control
    void runGameLoop();
    void processStep();
    bool isGameOver();
    void countDown();
    void endGame();

    // Actions and validations
    bool validateAction(Tank tank, Action action);
    bool validateShoot(Tank tank);
    void applyAction(Tank& tank, Action action);

    //for debug
    void printCurrentState();

private:  // Logging
    void logTankAction(const Tank& tank, Action action, bool success);
    void logGameOver(int winner);
    void logGameStart();
    void logShellMove(Shell& shell, pair<int,int> newPos);
    void logWallWeakened(const pair<int, int>& pos);
    void logWallDestroyed(const pair<int, int>& pos);
    void logShellsCollided(vector<Shell*> shells);
    void logShellHitTank(Shell& shell, Tank& tank);
    void logTankOnMine(Tank& tank, const pair<int, int>& pos);
    void logTankOnTank(Tank& tank, const pair<int, int>& pos);
    void handleWallCollision(Cell& cell);
	void writeOutput();
    string actionToString(Action);
    string directionToString(Direction);
};

#endif
