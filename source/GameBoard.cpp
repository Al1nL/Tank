#include "../headers/GameBoard.h"

/**
 * @brief Constructor.
 *
 * @param filePath Path to the input file containing the board layout.
 *  - Generates input_errors.txt if needed with error logs accorfing to the input and handle each error by avoiding\skipping\adding blank lines.
 * @throws invalid_argument if the file can't be opened, dimensions can't be read, or a player is missing.
 */
GameBoard::GameBoard(string filePath) {
	ifstream file(filePath);
	if (!file) {
		throw invalid_argument("File could not be opened");
	}

	ofstream errorLog("input_errors.txt");
	bool hasErrors = false;

	// Read the dimensions of the grid
	file >> width >> height;
	if (file.fail()) {
		throw invalid_argument("Failed to read board dimensions");
	}

	// Initialize the grid with the given dimensions
	grid.resize(height, vector<Cell*>(width));

	// Skip the newline after dimensions
	file.ignore();

	string line;
	bool usedTanks[2] = {false, false};

	for (int row = 0; row < height; ++row) {
		if (!getline(file, line)) {
			// Fill missing rows with empty spaces
			for (int col = 0; col < width; ++col) {
				grid[row][col] = new Cell(OccupierType::None, {row,col});
			}
			hasErrors = true;
			if (errorLog) errorLog << "Row " << row << " is missing. Filling with empty cells.\n";
			continue;
		}
		for (int col = 0; col < width; ++col) {
			char symbol = (col < line.size()) ? line[col] : ' ';

			// Set the occupier type and initialize the cell accordingly
			switch (symbol) {
			case '#':  // Wall
				grid[row][col] = new Cell(OccupierType::Wall, {row,col});
				break;
			case '1':  // Tank 1
			case '2': { // Tank 2
				int tankID = symbol - '0';
				if(usedTanks[tankID-1]) {
					grid[row][col] = new Cell(OccupierType::None, {row,col});
					hasErrors = true;
					if (errorLog) errorLog << "Duplicate tank " << tankID << " found at (" << row << ", " << col << "). Ignoring it.\n";
				} else {
					grid[row][col] = new Cell(OccupierType::Tank, {row,col}, tankID);
					usedTanks[tankID-1] = true;
				}
			}// Initialize Tank 1 at this cell if needed
			break;
			case '@':  // Mine
				grid[row][col] = new Cell(OccupierType::Mine, {row,col});
				break;
			case ' ':  // Empty space (None)
				grid[row][col] = new Cell(OccupierType::None, {row,col});
				break;
			default:
				hasErrors = true;
				if (errorLog) errorLog << "Unknown symbol '" << symbol << "' at (" << row << ", " << col << "). Treating as empty space.\n";
				grid[row][col] = new Cell(OccupierType::None, {row,col});
				break;
			}
		}

		// Fill missing columns in the row with empty spaces
		for (int col = line.size(); col < width; ++col) {
			grid[row][col] = new Cell(OccupierType::None, {row,col});
			hasErrors = true;
			if (errorLog) errorLog << "Column " << col << " in row " << row << " is missing. Filling with empty cell.\n";
		}

		if(line.size() > width) {
			hasErrors = true;
			if (errorLog) errorLog << line.size() - width << " Excess Columns " << "in row " << row << " found. Ignoring it.\n";
		}
	}

	// Handle excess rows
	int rowIndex = height;
	while (getline(file, line)) {
		hasErrors = true;
		++rowIndex;
	}
	if (errorLog && rowIndex - height >0) errorLog << rowIndex - height << " Excess rows " << " found. Ignoring it.\n";

	if (!hasErrors && errorLog) {
		errorLog.close(); // Delete file if no errors were logged
		remove("input_errors.txt");
	}

	if(!usedTanks[0] || !usedTanks[1]) {
		throw invalid_argument("Player missing. Game can not start!");
	}
}

/**
 * @brief Updates the list of fired shells and places/clears shells on the board.
 *
 * @param shell Pointer to the Shell object to be added or removed.
 * @param add If true, adds the shell to the list; if false, removes it and updates the board.
 */
void GameBoard::updateFiredShells(Shell* shell, bool add)
{
	auto pos = shell->getPos();
	Cell* cell=grid[pos.first][pos.second];
	if(!add) {
		allFiredShells.erase(remove(allFiredShells.begin(), allFiredShells.end(), shell), allFiredShells.end());
		cell->setShell(nullptr);
	}
	else {
		allFiredShells.push_back(shell);
		cell->setShell(shell);
	}
}

/**
 * @brief Gets the position of a tank on the board.
 *
 * @param tankId ID of the tank (1 or 2).
 * @return A pair (row, column) of the tank's location. Returns (-1, -1) if not found.
 */
pair<int, int> GameBoard::getTankPosition(int tankId) {
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			if (grid[row][col]->getOccupierType() == OccupierType::Tank) {
				if (tankId == grid[row][col]->getTank()) {
					return {row, col};
				}
			}
		}
	}
	return {-1, -1};  // invalid default position if tank not found
}

/**
 * @brief Updates the board after a Movable object (tank or shell) moves.
 *
 * @param oldPos The original position of the object.
 * @param newPos The new position of the object. If (-1, -1), the object is removed.
 */
void GameBoard::updateBoard(pair<int,int> oldPos, pair<int,int> newPos) {
	Cell old = *grid[oldPos.first][oldPos.second];

	if(newPos.first == -1 && newPos.second == -1) {
		grid[oldPos.first][oldPos.second]->destroyOccupier();
		return;
	}
	if(old.hasTank()) {
		grid[newPos.first][newPos.second]->setTank(old.getTank());
		grid[oldPos.first][oldPos.second]->destroyOccupier();
	}
	else if(old.hasShell()) {
		grid[newPos.first][newPos.second]->setShell(const_cast<Shell*>(old.getShell()));
		grid[oldPos.first][oldPos.second]->setShell(nullptr);
	}
}

/**
 * @brief Retrieves a sequence of Cells from a starting position in a specific direction.
 *
 * @param from Starting position
 * @param offset Direction offset
 * @param amount Number of Cells to retrieve.
 * @return Vector of pointers to the requested Cells.
 */
const vector<Cell*> GameBoard::getCells(pair<int,int> from, pair<int,int> offset,int amount) {
	vector<Cell*> cells;
	for(int i =1; i <= amount; i++) {
		cells.push_back(grid[positiveMod(from.first+offset.first*i, height)][positiveMod(from.second+offset.second*i, width)]);
	}
	return cells;
}

/**
 * @brief Destructor.
 *
 * Frees dynamically allocated memory for the grid and clears all fired shells.
 */
GameBoard::~GameBoard() {
	allFiredShells.clear();
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			delete grid[row][col];
		}
	}
}

/**
 * @brief Prints the current state of the board to standard output.
 *
 * Used for bonus text file of the game board throughout the game
 * Symbols:
 * - 'S' for a shell
 * - '1' or '2' for tanks
 * - '#' for walls
 * - '@' for mines
 * - ' ' for empty cells
 */
void GameBoard::printBoard() {
	OccupierType occupier;
	Cell* c;
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			c = grid[row][col];
			if(c->hasShell()) {
				cout << "S";
				continue;
			}
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
		cout <<endl;
	}
}