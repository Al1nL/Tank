#include "GameManager.h"
#include <string>

GameManager::GameManager(string filepath){
  board = new GameBoard(filepath);
  player1 = new Tank(1,board.getTankPosition(1));
  player2 = new Tank(1,board.getTankPosition(2));
}