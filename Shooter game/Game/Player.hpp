#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Entity.hpp"

class Player : public Entity {
private:
    float m_upVel;
    bool m_isGrounded;
    bool m_isJumping;
    float m_jumpingTimer;


    void setDidLand(const float groundH) {
        if(m_hitBox.top + m_hitBox.height >= groundH) {
            m_isGrounded = true;
        }
        m_isGrounded = false;
    }

    void handleMovement(const float &dt, const sf::Vector2f winSize) {
        if(keyDown(Left) && m_hitBox.left > 0)  move(dt, {-1.f,  0.f});
        if(keyDown(Right) && m_hitBox.left  + m_hitBox.width < winSize.x) move(dt, { 1.f,  0.f});
        //if(keyDown(Down))  move(dt, { 0.f,  1.f});
        if(keyDown(Up) && !m_isJumping) {
            m_isJumping = true;
            m_upVel = -1.f * m_movementSpeed /  10.f;
        }
        if(m_isJumping) {
            move(dt, {0.f, m_upVel * 0.1f});

            m_jumpingTimer += dt;
            m_upVel += m_gravity;
            //setDidLand(400.f);

            //if(m_isGrounded)
            if(m_hitBox.top + m_hitBox.height >= winSize.y - 100.f)
            {
                m_jumpingTimer = 0.f;
                m_isJumping = false;
                m_upVel = 0.f;

            }
        }
    }

    void handleShooting(const float &dt) {

    }
public:
    Player(): Entity("res/Player sprite2.png", sf::Vector2f(100.f, 300.f), sf::Vector2f(80, 96), 300.f) {
        m_upVel = 0.f;
        m_isJumping = true;
        m_jumpingTimer = 0.f;
        m_isGrounded = false;
    }

    void update(const float  &dt, const sf::Vector2f winSize) {
        handleMovement(dt, winSize);
        handleShooting(dt);
    }
};


#endif // PLAYER_HPP_INCLUDED
