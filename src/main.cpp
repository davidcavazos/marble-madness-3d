#include <iostream>

#include <iostream>
#include <cstdlib>
#include "game.hpp"

using namespace std;

int main(int, char**) {
    Game game("game");
    game.loadScene();
    game.bindControls();
    game.runGameLoop();
    return EXIT_SUCCESS;
}
