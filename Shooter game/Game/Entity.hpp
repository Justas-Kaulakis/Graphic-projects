#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#define keyDown(key) sf::Keyboard::isKeyPressed(sf::Keyboard:: key)


class Entity : public sf::Drawable {
private:
    sf::Texture m_Texture;
    sf::IntRect m_spriteRect;
    sf::RectangleShape box;

protected:

    sf::Sprite m_sprite;
    sf::FloatRect m_hitBox;
    float m_movementSpeed;
    float m_gravity;

    void move(const float &dt, const sf::Vector2f dir) {
        /// Set sprite to look into a direction
        if(dir.x > 0) m_sprite.setTextureRect(m_spriteRect);
        else if (dir.x < 0) m_sprite.setTextureRect(
            sf::IntRect(
                m_spriteRect.left + m_spriteRect.width,
                m_spriteRect.top,
                -m_spriteRect.width,
                m_spriteRect.height
            )
        );

        m_sprite.move(dir * dt * m_movementSpeed);
        m_hitBox.left += dir.x * dt * m_movementSpeed;
        m_hitBox.top += dir.y * dt * m_movementSpeed;

        box.setPosition(m_hitBox.left, m_hitBox.top);
    }


public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // You can draw other high-level objects
        target.draw(box, states);
        target.draw(m_sprite, states);
    }

    Entity(const std::string textPath, const sf::Vector2f pos, const sf::Vector2f dimensions, const float speed)
    : m_hitBox(pos, dimensions), m_spriteRect({0, 0}, sf::Vector2i(dimensions)) {

        m_Texture.loadFromFile(textPath);
        m_sprite.setTexture(m_Texture);
        m_sprite.setPosition(pos);
        m_movementSpeed = speed;
        m_gravity = 0.9f;

        box.setOutlineThickness(3.f);
        box.setOutlineColor(sf::Color::Red);
        box.setFillColor(sf::Color::Transparent);
        box.setSize({m_hitBox.width, m_hitBox.height});
        box.setPosition(m_hitBox.left, m_hitBox.top);
    }

    virtual void update(const float &dt, const sf::Vector2f winSize) = 0;


};

#endif // ENTITY_HPP_INCLUDED
