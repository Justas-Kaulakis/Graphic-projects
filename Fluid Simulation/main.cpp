#include <SFML/Graphics.hpp>
#include <iostream>
#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)


#include "Fluid.h"

using namespace std;

int main()
{
    int N = 64;
    int Scale = 10;
    Fluid fluid(N, Scale, 10.f, 0.2f, 0.5f/*, DrawProp::VELOCITY*/);
    sf::RenderWindow window(sf::VideoMode(N * Scale, N * Scale), "Fluid Simulation!", sf::Style::Titlebar | sf::Style::Close);

    sf::Vector2i pmPos(0, 0);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
              case sf::Event::Closed:
                window.close();
                break;
              case sf::Event::MouseMoved:
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    //cout << "Adding\n";
                    sf::Vector2i mPos = sf::Mouse::getPosition(window);
                    //cout << mPos.y << " " << mPos.x << endl;
                    fluid.addDensity(mPos / Scale, 255.f);
                    fluid.addVelocity(mPos / Scale, mPos.x - pmPos.x, mPos.y - pmPos.y);
                    pmPos = mPos;
                }
                break;
              case sf::Event::MouseButtonReleased:
                pmPos = {0, 0};
                break;
              case sf::Event::KeyPressed:
                break;

            }
        }

        fluid.step(clock.restart().asSeconds());
        fluid.fade(0.3f);

        window.clear();
        window.draw(fluid);
        window.display();
    }

    return 0;
}
