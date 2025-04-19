#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <vector>
#include <utility>
#include <string>
#include <iostream>
// #include "GameManager.h"
#include "Cell.h"


class Cell;
class GameManager;
using namespace std;

class GameBoard{
  GameManager& gameManager;  // Reference to GameManager
    int width=0;
    int height=0;
    vector<vector<Cell*>> grid;
public:
    GameBoard();
    GameBoard(string filePath, GameManager& gm);
    pair<int,int> getTankPosition(int tankId);
    Cell& at(pair<int,int> pos) const {    return *grid[pos.first][pos.second];}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void updateBoard(pair<int,int> oldPos, pair<int,int> newPos);
    vector<Cell*> getCells(pair<int,int> from, pair<int,int> offset,int amount);

    //for debug
    void printBoard();

    ~GameBoard();
};
#endif