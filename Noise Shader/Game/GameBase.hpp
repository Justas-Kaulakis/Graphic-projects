#ifndef GAME_BASE_HPP_INCLUDED
#define GAME_BASE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)

class GameBase {
protected:
    float m_Width, m_Height;
    sf::RenderWindow* m_Window;

    void initWindow(unsigned int W, unsigned int H, string title, bool not_resizable) {
        m_Width = (float)W;
        m_Height = (float)H;
        m_Window = (not_resizable)?
            new sf::RenderWindow(sf::VideoMode(m_Width, m_Height), title, sf::Style::Titlebar | sf::Style::Close)
          : new sf::RenderWindow(sf::VideoMode(m_Width, m_Height), title);
    }

public:

    bool isRunning() const {
        return m_Window->isOpen();
    }
    virtual void update() {
        printf("Base update()\n");
    }

    virtual void draw() {
        printf("Base draw()\n");
    }

};

#endif // GAME_BASE_HPP_INCLUDED

