#ifndef DOUBLE_PENDULUM_HPP_INCLUDED
#define DOUBLE_PENDULUM_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <time.h>

#define PI 3.1415926535f
const float g = 2.f;

class DBPendulum : public sf::Drawable {
private:
    /// ------------------- Pendulum variables
    sf::CircleShape pOrigin, p1, p2;

    float drag;

    float r1, r2;    /// Pendulum lengths
    float m1,  m2;   /// masses
    float a1,  a2;   /// angles

    float a1_v, a2_v; /// velocities


    void drawLine(sf::RenderTarget &target, sf::Vector2f p1, sf::Vector2f p2) const {
        sf::Vertex line[2] = {
            sf::Vertex(p1),
            sf::Vertex(p2)
        };

        target.draw(line, 2, sf::Lines);

    }

    float getRand() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);}


public:
    DBPendulum(sf::Vector2f oPoint = {0.f, 0.f},
               float length1 = 100.f, float length2 = 100.f,
               float mass1 = 20.f, float mass2 = 20.f, float drag = 1.f)
    : r1(length1), r2(length2), m1(mass1), m2(mass2),
    a1(PI * 2.f * getRand()), a2(PI * 2.f * getRand()),
    a1_v(0.f), a2_v(0.f), drag(drag) {

        //this->a1 = PI * 2.f * getRand();  this->a2 = PI * 2.f * getRand();

        pOrigin.setRadius(5.f);
        pOrigin.setFillColor(sf::Color::Green);
        pOrigin.setOrigin(5.f, 5.f);
        pOrigin.setPosition(oPoint);

        p1 = pOrigin, p2 = pOrigin;
        p1.setFillColor(sf::Color::White);
        p2.setFillColor(sf::Color::Red);

    }

    void setOrigin(const sf::Vector2f point) {
        pOrigin.setPosition(point);
    }

    void setDrag(const float drag) { this->drag = drag;}

    void update(const float &dt) {
        float num1 = -g * (2.0f * m1 + m2) * sin(a1);
        float num2 = -m2 * g * sin(a1 - 2.0f * a2);
        float num3 = -2.0f * sin(a1 - a2)*m2;
        float num4 = a2_v * a2_v*r2 + a1_v * a1_v*r1*cos(a1 - a2);
        float den = r1 * (2.0f * m1 + m2 - m2 * cos(2.0f * a1 - 2.0f * a2));
        float a1_a = (num1 + num2 + num3 * num4) / den;

        num1 = 2.0f * sin(a1 - a2);
        num2 = (a1_v*a1_v*r1*(m1 + m2));
        num3 = g * (m1 + m2) * cos(a1);
        num4 = a2_v * a2_v*r2*m2*cos(a1 - a2);
        den = r2 * (2.0f * m1 + m2 - m2 * cos(2.0f * a1 - 2.0f * a2));
        float a2_a = (num1*(num2 + num3 + num4)) / den;


        float x1 = pOrigin.getPosition().x + sin(a1) * r1;
        float y1 = pOrigin.getPosition().y + cos(a1) * r1;

        float x2 = x1 + sin(a2) * r2;
        float y2 = y1 + cos(a2) * r2;


        a1_v += a1_a * dt;
        a2_v += a2_a * dt;
        a1 += a1_v;
        a2 += a2_v;


        a1_v *= drag;
        a2_v *= drag;


        p1.setPosition(x1, y1);
        p2.setPosition(x2, y2);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        drawLine(target, pOrigin.getPosition(), p1.getPosition());
        drawLine(target, p1.getPosition(), p2.getPosition());

        target.draw(pOrigin);
        target.draw(p1);
        target.draw(p2);
    }

};


#endif // DOUBLE_PENDULUM_HPP_INCLUDED
