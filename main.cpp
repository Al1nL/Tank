#include "headers/GameManager.h"
#include <iostream>
#include <exception>

using namespace std;

int main(int argc, const char * argv[]) {
    // Initialize game manager and game board
    for(int i = 1; i < 11; i++) {
        cerr << "testing board number: " << i << endl;
        string filePath = "resources/board" + to_string(i) + ".txt";
        try {
            GameManager* gameManager = new GameManager(filePath);

            gameManager->printCurrentState();
            // Main game loop
            while (!gameManager->isGameOver()) {


                // Update game state
                gameManager->processStep();
                //for debug
                gameManager->printCurrentState();
            }

            // Log game over and results
            gameManager->endGame();
            std::cout << "Game Over for board " <<i<<". Check the log file for details.\n";
        }catch (const std::exception &e){
            cerr << "board number: " << i << "fail"  << endl;
            std::clog << e.what() << std::endl;
            return 1;
        }

    }
    return 0;
}
