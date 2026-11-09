/**
 * @file main.cpp
 * A simple program that runs Conway's Game of Life.
 */

#include "game.h"


/**
 * Main routine.
 * Reads in a starting configuration and simulates it, saving the results as a gif.
 */
int main() {

    Game* game = new Game(readFromFile("../data/Methuselahs/R-pent.txt", 100, 100, 45, 45), 500); // Change the starting configuration!
    game->run();
    Animation ani = game->animate(1);
    ani.write("R-pent.gif");
    
    #ifndef BUG_6
    delete game;
    #endif

    return 0;
}
