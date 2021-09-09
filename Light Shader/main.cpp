#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "VolumeLight2D.h"
#include "randomGen.h"

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)

using namespace std;

sf::ConvexShape getRandShape(RandomGen & Gen, float size) {
    int num = Gen.getInRange(1, 3);
    switch(num) {
    case 1:
        return createCircle(size / 2);
        break;
    case 2:
        return createSquare(size);
        break;
    case 3:
        return createSquare(size);
        break;
    }
}

int main() {
    RandomGen Gen;

    for(int i = 0; i < 10; i++)
        cout << Gen.getInRange(1, 6) << endl;

    float H = 500.f, W = 600.f;
    sf::RenderWindow window(sf::VideoMode((int)W, (int)H), "SHADER LIGHTS");

    vector<sf::ConvexShape> shapes;
    vector<PointLight> lights;
    PointLight mouseLight(sf::Vector2f(sf::Mouse::getPosition(window)), 200.f, Gen.getRandColor(), &W, &H);
    PointLight staticLight({100.f, 100.f}, 200.f, Gen.getRandColor(), &W, &H);

    sf::Vector2f mousePos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized: {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                W = event.size.width;
                H = event.size.height;
                break;
            }
            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    float size = 50.f;
                    sf::ConvexShape newShape = getRandShape(Gen, size);
                    newShape.setOrigin(size / 2, size / 2);
                    newShape.setPosition(mousePos);
                    newShape.setFillColor(Gen.getRandColor());
                    shapes.push_back(newShape);
                } else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    lights.push_back(mouseLight);
                    //cout << lights.size();
                    mouseLight.setColor(Gen.getRandColor());
                }

            case sf::Event::MouseMoved:
                mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                mouseLight.setSrcPosition(mousePos);
                mouseLight.calculateVisibilityPolygon(shapes);
                for(PointLight & light : lights)
                    light.calculateVisibilityPolygon(shapes);
                break;
            case sf::Event::MouseWheelMoved:
                mouseLight.setRadius(mouseLight.getRadius() + event.mouseWheel.delta * 15);
                mouseLight.calculateVisibilityPolygon(shapes);
                break;
            case sf::Event::KeyPressed:
                break;
            }
        }

        mouseLight.update();

        window.clear();

        for(PointLight & light : lights) {
            light.update();
            window.draw(light);
        }

        staticLight.update();
        window.draw(staticLight);

        for(sf::ConvexShape &shape : shapes)
            window.draw(shape);

        window.draw(mouseLight);

        window.display();
    }

    return 0;
}
