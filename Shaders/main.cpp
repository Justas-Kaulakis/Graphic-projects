#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

#include "Utility.hpp"

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)



void handleEvents(sf::RenderWindow & window, sf::Vector2f & mousePos) {
    sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
              case sf::Event::Closed:
                window.close();
                break;
              case sf::Event::Resized: {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                break;
              }
              case sf::Event::MouseMoved:
                mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                //mousePos.y *= -1.f;
                mousePos.y = window.getSize().y - mousePos.y;
            }
        }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "SHADERS!");

    sf::Texture text;
    text.create(600, 600);
    sf::Sprite sprite(text);

    sf::Shader noiseShader, voranoiShader;

    noiseShader.loadFromMemory(getJoinedShader("shaders/myNoise.glsl"), sf::Shader::Fragment);
    voranoiShader.loadFromMemory(getJoinedShader("shaders/myVoranoi.glsl"), sf::Shader::Fragment);

    sf::Vector2f mousePos;

    sf::Clock clock;
    while (window.isOpen())
    {
        handleEvents(window, mousePos);


        /// -------------------- UPDATING -------------------

        noiseShader.setUniform("u_time", clock.getElapsedTime().asSeconds());
        noiseShader.setUniform("u_resolution", sf::Vector2f(window.getSize()));


        voranoiShader.setUniform("u_mouse", mousePos);
        voranoiShader.setUniform("u_resolution", sf::Vector2f(window.getSize()));

        /// -------------------- DRAWING --------------------
        window.clear();

        //window.draw(sprite, &noiseShader);
        window.draw(sprite, &voranoiShader);

        window.display();
    }

    return 0;
}
