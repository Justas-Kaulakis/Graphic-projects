#include "Game/Game.hpp"
#include <iostream>


int main()
{
    Game game(400, 600, "Draw hopper", true);

    while (game.isRunning())
    {
        game.update();
        game.draw();
    }

    return 0;
}
