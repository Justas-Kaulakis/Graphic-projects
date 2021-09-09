#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <string>
#include "Collision.h"

enum class LookDir {Left, Right};

class Player : public sf::Drawable {
private:
    sf::Texture m_Texture;
    sf::Sprite m_Player;
    sf::Vector2i m_SpriteSize;

    LookDir m_CurrDir;
    sf::Vector2f m_Vel;  /// Where the entity is moving
    sf::Vector2f m_Acc;  /// force applied to the entity

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_Player, states);
    }

public:
    Player(std::string imgFile, sf::Vector2i spriteSize, sf::Vector2f startPsition, LookDir defDir = LookDir::Right)
    : m_SpriteSize(spriteSize),  m_CurrDir(defDir), m_Velocity({0.f, 0.f}) {
        Collision::CreateTextureAndBitmask(m_Texture, imgFile);
        m_Player.setTexture(m_Texture);
        m_Player.setPosition(startPsition);
        m_Player.setOrigin(sf::Vector2f(m_SpriteSize.x / 2, m_SpriteSize.y / 2));
    }

    bool testCollision(sf::Sprite& sprite) const {
        return Collision::PixelPerfectTest(m_Player, sprite);
    }

    /// \brief Sets which square rectangle will be the texture of the sprite
    ///
    void setTextOffset(unsigned int off) {
        int offset = (int)off;
        int sprCount = (int)(m_Texture.getSize().x) / m_SpriteSize.x;

        sf::Vector2i position = {(offset % sprCount) * m_SpriteSize.x,
                                 (offset / sprCount) * m_SpriteSize.y};

        m_Player.setTextureRect(sf::IntRect(position, m_SpriteSize));
    }

    void setLookDirection(const LookDir direction) {
        if(m_CurrDir != direction) {
            m_Player.scale(-1.f, 1.f);
            if(m_CurrDir == LookDir::Left) m_CurrDir = LookDir::Right;
            else                           m_CurrDir = LookDir::Left;
        }
    }

    void applyForce(const sf::Vector2f force) {
        m_Acc = force;
    }

    void update() {
        m_Vel += m_Acc;
        m_Player.move(m_Vel);
    }

};

#endif // PLAYER_HPP_INCLUDED
