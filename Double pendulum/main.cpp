#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <time.h>
#include "Double_pendulum.hpp"

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)



int main() {
    int H = 600, W = 600;
    sf::RenderWindow window(sf::VideoMode(H, W), "Double Pendulum!");
    window.setView(sf::View(sf::Vector2f(0.f, H * 0.5f - 200.f), sf::Vector2f(W, H)));
    window.setVerticalSyncEnabled(true);


    srand(time(0));

    std::vector<DBPendulum> pendulums;//(2, DBPendulum({0.f, 0.f}, 200.f, 150.f, 50.f, 150.f, 0.9999f));

    for(int i = 0; i < 2; i++)
        pendulums.push_back(DBPendulum({0.f, 0.f}, 200.f, 150.f, 50.f, 150.f, 0.99f));



    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds() * 10.f;
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized: {
                window.setView(sf::View(sf::Vector2f(0.f, event.size.height * 0.5f - 200.f), sf::Vector2f(event.size.width, event.size.height)));
                break;
            }
            case sf::Event::KeyPressed:
                break;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            for(DBPendulum &pen : pendulums)
                pen.setOrigin(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window))));
        }


        for(DBPendulum &pen : pendulums)
            pen.update(dt);

        window.clear();

        for(DBPendulum &pen : pendulums)
            window.draw(pen);

        window.display();
    }

    return 0;
}
