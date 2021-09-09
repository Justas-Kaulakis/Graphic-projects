#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)
#define PI 3.1415926535f
#define R2G (360.f / (PI * 2.f))

using namespace std;

void drawLine(sf::RenderWindow &target, sf::Vector2f p1, sf::Vector2f p2) {
        sf::Vertex line[2] = {
            sf::Vertex(p1),
            sf::Vertex(p2)
        };

        target.draw(line, 2, sf::Lines);

}

float VecLength(sf::Vector2f p1, sf::Vector2f p2) {
    return sqrt((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y));
}


int main()
{
    int W = 700.f, H = 600.f;
    sf::RenderWindow window(sf::VideoMode(700, 600), "Inverse Kinematics ARM!");

    sf::CircleShape pOrigin, p1, p2;


    pOrigin.setRadius(5.f);
    pOrigin.setFillColor(sf::Color::Green);
    pOrigin.setOrigin(5.f, 5.f);
    pOrigin.setPosition(W/2, H/2);

    p1 = pOrigin, p2 = pOrigin;
    p1.setFillColor(sf::Color::White);
    p2.setFillColor(sf::Color::Red);

    float length0 = 200.f, length1 = 200.f;
    sf::Vector2f mousePos = {100.f, 200.f};

    float a1 = 0.f;
    float a2 = 0.f;



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
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                break;
              }
              case sf::Event::KeyPressed:
                break;

            }
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        }

        // Distance from Joint0 to Target
        float length2 = VecLength(pOrigin.getPosition(), mousePos);

        sf::Vector2f diff = mousePos - pOrigin.getPosition();
        float atan = atan2(diff.y, diff.x);



        if (length0 + length1 < length2) {
            a1 = atan;
            a2 = atan;
        }
        else {
            // Inner angle alpha
            float cosAngle0 = ((length2 * length2) + (length0 * length0) - (length1 * length1)) / (2 * length2 * length0);
            float angle0 = acos(cosAngle0);

            // Inner angle beta
            float cosAngle1 = ((length1 * length1) + (length0 * length0) - (length2 * length2)) / (2 * length1 * length0);
            float angle1 = acos(cosAngle1);

            // In radians
            a1 = atan - angle0;    // Angle A
            a2 = PI - angle1 + a1;    // Angle B
        }



        float x1 = pOrigin.getPosition().x + cos(a1) * length0;
        float y1 = pOrigin.getPosition().y + sin(a1) * length0;

        float x2 = x1 + cos(a2) * length1;
        float y2 = y1 + sin(a2) * length1;


        p1.setPosition(x1, y1);
        p2.setPosition(x2, y2);


        window.clear();

        drawLine(window, pOrigin.getPosition(), p1.getPosition());
        drawLine(window, p1.getPosition(), p2.getPosition());

        window.draw(pOrigin);
        window.draw(p1);
        window.draw(p2);

        window.display();
    }

    return 0;
}
