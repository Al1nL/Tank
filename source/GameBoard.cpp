#include "../headers/GameBoard.h"

GameBoard::GameBoard(string filePath,GameManager& gm): gameManager(gm){
    ifstream file(filePath);
    if (!file) {
        cerr << "Unable to open file: " << filePath << endl;
        exit(1);  // Handle error appropriately
    }

    // Read the dimensions of the grid
    file >> width >> height;

    // Initialize the grid with the given dimensions
    grid.resize(height, vector<Cell*>(width));

    // Skip the newline after dimensions
    file.ignore();

    string line;
    for (int row = 0; row < height; ++row) {
        getline(file, line);
        for (int col = 0; col < width; ++col) {
            char symbol = line[col];

            // Set the occupier type and initialize the cell accordingly
            switch (symbol) {
                case '#':  // Wall
                    grid[row][col] = new Cell(OccupierType::Wall,gameManager);
                break;
                case '1':  // Tank 1
                    grid[row][col] = new Cell(OccupierType::Tank,gameManager, 1);
                // Initialize Tank 1 at this cell if needed
                break;
                case '2':  // Tank 2
                    grid[row][col] = new Cell(OccupierType::Tank,gameManager,2);
                // Initialize Tank 2 at this cell if needed
                break;
                case '@':  // Mine
                    grid[row][col] = new Cell(OccupierType::Mine,gameManager);
                break;
                case ' ':  // Empty space (None)
                    grid[row][col] = new Cell(OccupierType::None,gameManager);
                break;
                default:
                    cerr << "Unknown symbol: " << symbol << " at position (" << row << "," << col << ")" << endl;
                break;
            }
        }
    }
}
pair<int, int> GameBoard::getTankPosition(int tankId) {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (grid[row][col]->getOccupierType() == OccupierType::Tank) {
                if (tankId == grid[row][col]->getTank()) {
                    return {row, col};
                }

//                else if (tankId == 2 && grid[row][col]->getTank() == 2) {
//                    return {row, col};
//                }
            }
        }
    }
    return {-1, -1};  // Return an invalid position if tank not found
}

void GameBoard::updateBoard(pair<int,int> oldPos, pair<int,int> newPos){
  	Cell old = *grid[oldPos.first][oldPos.second];
	OccupierType oldOccupier=  grid[oldPos.first][oldPos.second]->getOccupierType();
//
    if(newPos.first != -1 && newPos.second != -1){

    	switch (oldOccupier){
      		case OccupierType::Tank:
        		grid[newPos.first][newPos.second]->setTank(old.getTank());
    			break;
            case OccupierType::Wall:
              	grid[newPos.first][newPos.second]->damageWall();
                break;

        }
        if(old.hasShell()){
             grid[newPos.first][newPos.second]->setShell(old.getShell());
        }
    }
    grid[oldPos.first][oldPos.second]->destroyOccupier();

//	for (int row = 0; row < height; ++row) {
//        for (int col = 0; col < width; ++col) {
//            oldOccupier = grid[row][col]->getOccupierType();
//            switch (oldOccupier){
//      			case OccupierType::Tank:
//        			grid[newPos.first][newPos.second]->setTank(old.getTank());
//    				break;
//            	case OccupierType::Wall:
//              		grid[newPos.first][newPos.second]->damageWall();
//                	break;
//            	case OccupierType::Shell:
//              		grid[newPos.first][newPos.second]->setShell(old.getShell());
//                	break;
//        	}
//
//        }
//    }
}

vector<Cell*> GameBoard::getCells(pair<int,int> from, pair<int,int> offset,int amount){
  vector<Cell*> cells;
  for(int i =1; i <= amount; i++){
    	cells.push_back(grid[from.first+offset.first*i][from.second+offset.second*i]);
    }
  return cells;
}

GameBoard::~GameBoard(){
   for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
          delete grid[row][col];
        }
   }
}

void GameBoard::printBoard(){
  OccupierType occupier;
  Cell* c;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      c = grid[row][col];
      if(c->hasShell()){ cout << "S"; continue;}
      occupier=  c->getOccupierType();

      switch (occupier) {
        case OccupierType::Tank:
          	cout << c->getTank();
            break;
        case OccupierType::Wall:
            cout << "#";
            break;
        case OccupierType::None:
            cout << " ";
            break;
        case OccupierType::Mine:
          	cout << "@";
            break;
      }
    }
    cout << endl;
  }
}