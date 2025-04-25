#ifndef GAMEMANAGER_H  // Header guard to prevent multiple inclusions
#define GAMEMANAGER_H
#include <vector>
#include <map>
#include <set>
#include <algorithm> //for find func
#include <utility>
#include <iostream>
#include <fstream>
#include "Action.h"
#include "GameBoard.h"
#include "Tank.h"
#include <string>
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
    // int gameOverSign=0; // 0: no game over, 1: one of players won(indicate by who lost), 2: Tie

  public:
    GameManager();
    GameManager(string filepath);

    void runGameLoop();
    void processStep();
    bool validateAction(Tank tank, Action action);
    bool validateShoot(Tank tank);
    void applyAction(Tank& tank, Action action);
    void goThroughCells(vector<Cell*> cells, Shell* shell);

    bool isGameOver();
    void countDown();
    void endGame();

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
	void writeOutput();
    string actionToString(Action);
    string directionToString(Direction);

    //void moveFiredShells(Tank&);
    //for debug
    void printCurrentState();

    void moveFiredShells();
    void removeShellFromGame(Shell* shell, vector<Shell*>& allShells,map<pair<int, int>, vector<Shell*>> &cellToShells);
    void handleShellCollision(vector<Shell*>& allShells, map<Shell*,pair<int,int>>& previousPositions,int step,map<pair<int, int>, vector<Shell*>> &cellToShells);
    vector<Shell*> getAllFiredShells();
    void updateShellPositions(vector<Shell*>& allShells, map<Shell*, pair<int, int>>& previousPositions, map<pair<int, int>, vector<Shell*>>& cellToShells);




};
#endif
