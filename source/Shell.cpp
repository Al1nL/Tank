#include "../headers/Shell.h"

//todo: ±2 for either or both x and y, depending on direction, without skipping squares
//bool Shell::goThroughCells(vector<Cell*> cells) {
//    OccupierType occupier;
//	for (Cell* c : cells) {
//		occupier = c->getOccupierType();
//		if(c->hasShell()) {
//			c->getShell()->getOwnerID() == 1 ? deleteShell(c->getShell()) : deleteShell(c->getShell());
//
//		}
//		switch (occupier) {
//		case OccupierType::Tank:
//			break;
//		case OccupierType::Wall:
//            c->damageWall();
//			break;
//
//		}
//	}
//    return false;
//}

//need to implement
bool Shell::isValidMove(const GameBoard& board) {
        return true;  // or whatever you want
}

//void Shell::move(GameBoard& board) {
//  vector<Cell*> cells = board.getCells(getPos(), getOffset(), getSteps());
//	goThroughCells(cells);
//    board.updateBoard(getPos(), getOffset());
//}

