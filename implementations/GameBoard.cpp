#include "GameBoard.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
                    grid[row][col] = new Cell(OccupierType::Tank,gameManager);
                // Initialize Tank 1 at this cell if needed
                break;
                case '2':  // Tank 2
                    grid[row][col] = new Cell(OccupierType::Tank,gameManager);
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
pair<int, int> GameBoard::getTankPosition(int tankId) const {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (grid[row][col]->getOccupierType() == OccupierType::Tank) {
                if (tankId == 1 && grid[row][col]->getTank()->getID() == 1) {
                    return {row, col};
                } else if (tankId == 2 && grid[row][col]->getTank()->getID() == 2) {
                    return {row, col};
                }
            }
        }
    }
    return {-1, -1};  // Return an invalid position if tank not found
}