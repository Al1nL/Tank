#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <vector>
#include <utility>
#include "Cell.h"
class Cell;

class GameBoard{
    int width=0;
    int height=0;
    vector<vector<Cell>> grid;
public:
    GameBoard(string filePath);
    GameBoard(){};
    pair<int,int> getTankPosition(int tankId) const;
    Cell& at(vector<int> pos){return grid[pos[0]][pos[1]];}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
#endif