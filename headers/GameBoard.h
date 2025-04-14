#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <vector>
#include <utility>
#include "Cell.h"
#include <string>
#include <iostream>

class Cell;

class GameBoard{
  GameManager& gameManager;  // Reference to GameManager
    int width=0;
    int height=0;
    vector<vector<Cell*>> grid;
public:
    GameBoard(string filePath, GameManager& gm);
    pair<int,int> getTankPosition(int tankId) const;
    Cell& at(vector<int> pos){    return *grid[pos[0]][pos[1]];}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
#endif