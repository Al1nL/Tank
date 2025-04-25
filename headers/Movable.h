#ifndef MOVABLE_H
#define MOVABLE_H
#include <utility> //for pair
#include <vector>
#include <string>
using namespace std;

enum Direction { U, UR, R, DR, D, DL, L, UL, None};
static const vector<pair<int, int>> offsets = {
  {-1, 0},  // U
  {-1, 1},  // UR
  { 0, 1},  // R
  { 1, 1},  // DR
  { 1, 0},  // D
  { 1, -1}, // DL
  { 0, -1}, // L
  {-1, -1}  // UL
};

class Movable{
  protected:
    pair<int,int> pos;
    Direction dir;
	  int perStep;

  public:
    Movable():pos({-1,-1}),dir(Direction::L),perStep(0){}
    Movable(pair<int,int> pos, Direction dir, int step) : pos(pos), dir(dir), perStep(step){};
    int wrap(int value, int size) { return (value % size + size) % size; }; // lambda to wrap-around

    pair<int,int> nextStep(bool forward,const int rows, const int cols){
        // int side = forward ? 1 : -1;* side* side


        int newRow = wrap(pos.first + perStep * offsets[dir].first , rows);
        int newCol = wrap(pos.second + perStep * offsets[dir].second ,cols);
        return {newRow , newCol };
      }
    string getPosition() const {return "[" + to_string(pos.first) + "," + to_string(pos.second) + "]";}
	pair<int,int> getPos() const {return pos;}
    void setPos(pair<int,int> posi) { this->pos = posi; }
    Direction getDir() const { return dir; }
    void setDir(Direction d) { dir = d; }
    int getSteps() const { return perStep; }
  };


  #endif