#ifndef GAME_BASE_HPP_INCLUDED
#define GAME_BASE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

class GameBase {
protected:
    float m_Width, m_Height;
    sf::RenderWindow* m_Window;

public:
    GameBase(unsigned int W, unsigned int H, string title, bool not_resizable) {
        m_Width = (float)W;
        m_Height = (float)H;
        m_Window = (not_resizable)?
            new sf::RenderWindow(sf::VideoMode(m_Width, m_Height), title, sf::Style::Titlebar | sf::Style::Close)
          : new sf::RenderWindow(sf::VideoMode(m_Width, m_Height), title);
          m_Window->setVerticalSyncEnabled(true);
    }

    bool isRunning() const {
        return m_Window->isOpen();
    }
    virtual void update() = 0;
    virtual void draw() = 0;

};

#endif // GAME_BASE_HPP_INCLUDED

