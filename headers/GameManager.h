#ifndef GAMEMANAGER_H  // Header guard to prevent multiple inclusions
#define GAMEMANAGER_H
#include <vector>
#include "GameBoard.h"
class Tank;

class GameManager{
    GameBoard board;
    Tank player1;
    Tank player2;
    int stepCounter=0;
    vector<string> logs;
public:
    GameManager();
    GameManager(string filepath);
    void loadBoard(string filepath);
    void runGameLoop();
    void processStep();
    void writeOutput(string filename);
    void detectCollision();
};
#endif
