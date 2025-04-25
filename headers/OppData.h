#ifndef OPPDATA_H
#define OPPDATA_H
#include <utility>

class Movable;

using namespace std;

struct OppData{
    const pair<int,int> opponentPos;
    const Direction opponentDir;
};
#endif //OPPDATA_H
