#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main()
{
    float Width = 800.f, Height = 500.f;
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML works!");

    vector<bool> isPressed((int)sf::Keyboard::KeyCount);

    sf::CircleShape point(2.f);
    point.setOrigin(point.getRadius(), point.getRadius());
    point.setFillColor(sf::Color::Red);

    sf::Texture tex;
	tex.create(Width, Height);
	sf::Sprite spr(tex);


    sf::Shader shader;
	shader.loadFromFile("shader.glsl", sf::Shader::Fragment); // load the shader



    sf::Vector2f zoomCenter(-0.5, 0.f);
    float zoomSize = 4.f;
    int maxIterations = 100;
    sf::Vector2f mousePosition;
    sf::Vector2u lastSize;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
              case sf::Event::Closed: window.close();
                break;
              case sf::Event::Resized: {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                break;
              }
              case sf::Event::KeyPressed:
                isPressed[event.key.code] = true;
                if(isPressed[sf::Keyboard::X]) maxIterations += 1;
                else if(isPressed[sf::Keyboard::Z])
                    maxIterations = (maxIterations > 0)? maxIterations - 1 : 0;
                break;
              case sf::Event::KeyReleased:
                isPressed[event.key.code] = false;
                break;
              case sf::Event::MouseMoved:
                mousePosition.x = (float)(event.mouseMove.x / Width) - 0.5f;
                mousePosition.y = (float)(event.mouseMove.y / Height) - 0.5f;
                mousePosition *= 2.f;
                break;
            }
        }

        {
        if(isPressed[sf::Keyboard::Subtract]) zoomSize /= 0.99f;
        else if(isPressed[sf::Keyboard::Add])
            zoomSize = (zoomSize > 0.f)? zoomSize * 0.99f : 0.f;
        if(isPressed[sf::Keyboard::Up]) zoomCenter.y += zoomSize / 1000;
        else if(isPressed[sf::Keyboard::Down]) zoomCenter.y -= zoomSize / 1000;
        if(isPressed[sf::Keyboard::Right]) zoomCenter.x += zoomSize / 1000;
        else if(isPressed[sf::Keyboard::Left]) zoomCenter.x -= zoomSize / 1000;
        if(isPressed[sf::Keyboard::C]) {
            zoomSize = 1.f;
            zoomCenter = {0.f, 0.f};

        }
        }

        Height = (float)window.getSize().y;
        Width = (float)window.getSize().x;


        shader.setUniform("u_resolution", sf::Vector2f(Width, Height));
        shader.setUniform("u_zoomCenter", zoomCenter);
        shader.setUniform("u_maxIterations", maxIterations);
        shader.setUniform("u_zoomSize", zoomSize);


        point.setPosition(Width / 2, Height / 2);

        window.clear();

        window.draw(spr, &shader);
        window.draw(point);
        window.display();
    }

    return 0;
}
