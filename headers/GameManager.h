#ifndef GAMEMANAGER_H  // Header guard to prevent multiple inclusions
#define GAMEMANAGER_H
#include <vector>
#include <utility>
#include "Tank.h"
#include <fstream>
#include "Action.h"

using namespace std;
class GameBoard;

class GameManager{
    GameBoard* currGameState;
    Tank* player1;
    Tank* player2;
    int stepCounter=0;
    int stepsSinceNoShells = 0;
    vector<string> logs;
    std::ofstream logFile;

    bool p1Lost=false;
    bool p2Lost=false;
    int gameOverSign=0; // 0: no game over, 1: one of players won(indicate by who lost), 2: Tie

  public:
    GameManager();
    GameManager(string filepath);

    void runGameLoop();
    void processStep();
    bool validateAction(Tank& tank, Action action);
    bool validateShoot(Tank& tank);
    void applyAction(Tank& tank, Action action);

    bool isGameOver();
    void countDown();

    void logTankAction(const Tank& tank, Action action, bool success);
    void logGameOver();
    void logGameStart();
void logShellAction(Shell&, const std::string&);
    void logWallWeakened(const pair<int, int>& pos);
    void logWallDestroyed(const pair<int, int>& pos);
    void logShellsCollided(Shell& shell1, Shell& shell2);
    void logShellHitTank(Shell& shell, Tank& tank);
	void writeOutput();
    string actionToString(Action);

};
#endif
