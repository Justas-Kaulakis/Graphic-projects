#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "GameBase.hpp"
#include "Utility.hpp"
#include <iostream>
#include <string>


class Game : public GameBase {
private:
    sf::Shader shader;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock clock;

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
                break;
            }
        }
    }
    void handleInput() {
        if(keyDown(Up));
    }

public:
    Game(unsigned int W, unsigned int H, string title, bool not_resizable = false) {
        initWindow(W, H, title, not_resizable);

        shader.loadFromMemory(getJoinedShader("shader.glsl"), sf::Shader::Fragment);


        texture.create(m_Width, m_Height);

        sprite.setTexture(texture);
    }

    void update() {
        handleEvents();
        m_Width = (float)m_Window->getSize().x;
        m_Height = (float)m_Window->getSize().y;
        handleInput();

        sprite.setScale(
                        m_Width / sprite.getLocalBounds().width,
                        m_Height / sprite.getLocalBounds().height);


        shader.setUniform("u_seed", clock.getElapsedTime().asSeconds());
        shader.setUniform("u_resolution", sf::Vector2f(m_Width, m_Height));
    }
    void draw() {
        m_Window->clear();

        sf::CircleShape c(50.f);



        m_Window->draw(sprite, &shader);
        m_Window->display();
    }
};

#endif // GAME_HPP_INCLUDED
