#include "Game/Game.hpp"
#include <iostream>


int main()
{
    Game game(300, 300, "Title");

    while (game.isRunning())
    {
        game.update();
        game.draw();
    }

    return 0;
}
