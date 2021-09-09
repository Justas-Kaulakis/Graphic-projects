#include "Game/Game.hpp"
#include <iostream>


int main()
{
    Game game(800, 500, "Shooter game");

    while (game.isRunning())
    {
        game.update();
        game.draw();
    }

    return 0;
}
