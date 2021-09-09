#ifndef INPUTMANAGER_HPP_INCLUDED
#define INPUTMANAGER_HPP_INCLUDED

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class InputMng {
private:
    sf::Event m_event;
    InputMng() {}
public:
    static void update(sf::Event event) {
        m_event = event;
    }
    static bool isKeyPressed(sf::Keyboard::Key key) {
        return (m_event.key.code == key);
    }

    static bool isKeyDown(sf::Keyboard::Key key) {
        return sf::Keyboard::isKeyPressed(key);
    }
};


#endif // INPUTMANAGER_HPP_INCLUDED
