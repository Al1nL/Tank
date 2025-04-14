#ifndef MOVABLE_H
#define MOVABLE_H
#include <utility> //for pair
#include <vector>
#include <string>
using namespace std;

class Movable{
  protected:
    pair<int,int> pos;

  public:
    Movable(pair<int,int> pos) : pos(pos) {};
    enum Direction { U, UR, R, DR, D, DL, L, UL };
    pair<int,int> nextStep(int step, Direction dir,const int rows,const int cols){
        static const vector<pair<int, int>> offsets = { // to match dir
          {1, 0}, {1, 1}, {0, 1}, {-1, 1},
          {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
        };
        auto wrap = [](int value, int size) { return (value % size + size) % size; }; // lambda to wrap-around

        int newRow = wrap(pos.first + step * offsets[dir].first, rows);
        int newCol = wrap(pos.second + step * offsets[dir].second, cols);
        return {newRow, newCol};
      }
          string getPosition() const {return "[" + to_string(pos.first) + "," + to_string(pos.second) + "]";}

  };
  #endif