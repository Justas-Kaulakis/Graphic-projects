#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "GameBase.hpp"
#include "Player.hpp"
#include "InputManager.hpp"
#include <iostream>



class Game : public GameBase {
private:
    sf::Clock m_clock;

    Player m_player;

    void handleEvents() {
        sf::Event event;
        while (m_Window->pollEvent(event))
        {
            switch(event.type) {
              case sf::Event::Closed:
                m_Window->close();
                break;
              case sf::Event::Resized: {
                m_Window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                break;
              }
              case sf::Event::KeyPressed:
                cout << "Updating\n";
                InputMng::update(event);
                break;
            }
        }
    }
public:
    Game(unsigned int W, unsigned int H, string title, bool not_resizable = false)
       : GameBase(W, H, title, not_resizable)  {

    }

    void update() {
        float dt = m_clock.restart().asSeconds();
        handleEvents();
        m_Width = (float)m_Window->getSize().x;
        m_Height = (float)m_Window->getSize().y;


        m_player.update(dt, {m_Width, m_Height});
    }
    void draw() {
        m_Window->clear(sf::Color::Cyan);

        sf::RectangleShape groundShape({m_Width, 100.f});

        groundShape.setPosition(0.f, 400);
        groundShape.setFillColor(sf::Color(92, 32, 3));

        m_Window->draw(groundShape);

        m_Window->draw(m_player);

        m_Window->display();
    }
};

#endif // GAME_HPP_INCLUDED
