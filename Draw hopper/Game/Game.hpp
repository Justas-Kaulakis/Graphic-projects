#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "GameBase.hpp"
#include "Mouse.hpp"
#include "Player.hpp"
#include <iostream>
#include <vector>

class Game : public GameBase {
private:
    Mouse m_Mouse;

    sf::Clock m_Clock;
    float m_DTime;

    sf::RenderTexture m_RenderTexture;
    sf::Texture m_CanvasText;
    sf::Sprite m_CanvasSpr;
    std::vector<sf::Vertex> line;


    Player m_Player;


    void handleEvents() {
        sf::Event event;
        while (m_Window->pollEvent(event))
        {
            switch(event.type) {
              case sf::Event::Closed:
                m_Window->close();
                break;
              case sf::Event::KeyPressed:
                break;
              case sf::Event::MouseButtonPressed:
                m_Mouse.setPressed(*m_Window);
                break;
              case sf::Event::MouseButtonReleased:
                m_Mouse.setDefault(*m_Window);

                m_CanvasText.update(m_RenderTexture.getTexture());
                line.clear();
                break;
              case sf::Event::MouseMoved:
                if(m_Mouse.IsPressed()) {
                    line.push_back(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(*m_Window))));
                }
                break;
            }
        }
    }
    void handleInput() {
        if(keyDown(Right)) {
            m_Player.move({ 1.f * m_DTime, 0});
            m_Player.setLookDirection(LookDir::Right);
        }
        if(keyDown(Left)) {
            m_Player.move({-1.f * m_DTime, 0});
            m_Player.setLookDirection(LookDir::Left);
        }
    }

public:
    Game(unsigned int W, unsigned int H, string title, bool not_resizable = false)
    //: m_Player("res/Player sprite.png", sf::Vector2i(60, 80), sf::Vector2f(200.f, 300.f))
    : m_Player("res/Player sprite2.png", sf::Vector2i(80, 96), sf::Vector2f(200.f, 300.f))

    {
        initWindow(W, H, title, not_resizable);
        setMaxFPS(60);
        m_Mouse.setDefault(*m_Window);

        if(!m_RenderTexture.create((unsigned)m_Width, (unsigned)m_Height))
            std::cout << "Couldn't create the canvas RenderTexture" << std::endl;

        m_CanvasText.create((unsigned)m_Width, (unsigned)m_Height);
        m_CanvasSpr.setTexture(m_CanvasText);
    }

    void update() {
        m_DTime = m_Clock.restart().asMilliseconds();
        handleEvents();
        handleInput();

        m_Player.update();

        m_CanvasText.update(m_RenderTexture.getTexture());
    }
    void draw() {
        m_RenderTexture.clear(sf::Color(0,0,0,0));   /// Drawing on to the canvas with a Render texture
        m_RenderTexture.draw(m_CanvasSpr);
        m_RenderTexture.draw(&line[0], line.size(), sf::LineStrip);
        m_RenderTexture.display();

        sf::Sprite tempSpr(m_RenderTexture.getTexture());

        m_Window->clear(sf::Color(0,0,0,0));

        m_Window->draw(tempSpr);                    /// Drawing the rendered canvas texture
        m_Window->draw(m_Player);                   /// drawing the player

        m_Window->display();
    }
};

#endif // GAME_HPP_INCLUDED
