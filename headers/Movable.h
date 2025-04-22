#ifndef MOVABLE_H
#define MOVABLE_H
#include <utility> //for pair
#include <vector>
#include <string>
using namespace std;

enum Direction { U, UR, R, DR, D, DL, L, UL };
static const vector<pair<int, int>> offsets = { // to match dir
        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };
class Movable{
  protected:
    pair<int,int> pos;
    Direction dir;
	  int perStep;

  public:
    Movable(pair<int,int> pos, Direction dir, int step) : pos(pos), dir(dir), perStep(step){};

    pair<int,int> nextStep(bool forward,const int rows){
        int side = forward ? -1 : 1;

        // auto wrap = [](int value, int size) { return (value % size + size) % size; }; // lambda to wrap-around

        int newRow = pos.first + perStep * offsets[dir].first * side;
        int newCol = pos.second + perStep * offsets[dir].second * side;
        return {newRow % rows, newCol};
      }
    string getPosition() const {return "[" + to_string(pos.first) + "," + to_string(pos.second) + "]";}
	pair<int,int> getPos() const {return pos;}
    void setPos(pair<int,int> posi) { this->pos = posi; }
    Direction getDir() const { return dir; }
    void setDir(Direction d) { dir = d; }
    int getSteps() const { return perStep; }
  };


  #endif