#include <SFML/Graphics.hpp>
#include <iostream>
#include "FourierSieries.h"
#include "CreatePath.h"

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)

int main()
{
    float W = 600.f, H = 500.f;
    sf::RenderWindow window(sf::VideoMode(W, H), "CIRCLES DRAW!");
    window.setVerticalSyncEnabled(true);
    window.setView(sf::View(sf::FloatRect(sf::Vector2f(-(W / 2.f), -(H / 2.f)), sf::Vector2f(W, H))));
    window.setFramerateLimit(60);

    CreatePath createPath;


    vector<float> signal;
    for(int i = 0; i < 100; i++)
    {
        signal.push_back((float)i* 5);
    }
    FourierSeries FS("bart.txt");

    sf::Clock clock;


    float scale = 1.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
              case sf::Event::Closed:
                window.close();
                break;
              case sf::Event::Resized: {
                W = event.size.width; H = event.size.height;
                window.setView(sf::View(sf::FloatRect(
                    sf::Vector2f(-(W / 2.f), -(H / 2.f)),
                    sf::Vector2f(W,H)
                )));
                break;
              }
              case sf::Event::MouseWheelMoved:
                scale += (float)event.mouseWheel.delta / 10.f;
                FS.setScale(scale);
                break;
              case sf::Event::MouseButtonPressed:
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    int x = sf::Mouse::getPosition(window).x - W / 2;
                    int y = sf::Mouse::getPosition(window).y - H / 2;
                    createPath.addPoint(sf::Vector2i(x, y));
                }
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    createPath.stopDraw();
                    FS.setPath(createPath.getPath());
                    FS.startDraw();
                }
            }
        }

        window.clear();
        // clock.getElapsedTime().asSeconds()
        window.draw(createPath);
        FS.draw(window);
        window.display();
    }

    return 0;
}
