#include "headers/GameManager.h"
#include <iostream>
#include <exception>

using namespace std;

int main(int argc, const char * argv[]) {
    // Initialize game manager and game board
    string filePath = "";
    if(argc > 1) {
        filePath = argv[1];
    }
    try {
        GameManager* gameManager = new GameManager(filePath);
        gameManager->runGameLoop();
        delete gameManager;
        std::cout << "Game Over. Check the log file for details.\n";
    }catch (const std::exception &e){

        std::clog << e.what() << std::endl;
        return 1;
    }
    return 0;
}