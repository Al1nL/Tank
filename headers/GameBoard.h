#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <vector>
#include <algorithm> //for remove func
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include "Cell.h"


class Cell;
using namespace std;

class GameBoard{
    int width=0;
    int height=0;
    vector<vector<Cell*>> grid;
    vector<Shell*> allFiredShells;

public:
    GameBoard();
    GameBoard(string filePath);
    pair<int,int> getTankPosition(int tankId);
    Cell& at(pair<int,int> pos) const {    return *grid[pos.first][pos.second];}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const vector<Shell*> getAllFiredShells() const {return allFiredShells;}
    void updateFiredShells(Shell* s,bool add);
    void updateBoard(pair<int,int> oldPos, pair<int,int> newPos);
    vector<Cell*> getCells(pair<int,int> from, pair<int,int> offset,int amount);
    // int positiveMod(int dividend, int divisor) const{
    //     return ((dividend % divisor) + divisor) % divisor;
    // }
    //for debug
    void printBoard();

    ~GameBoard();
};
#endif