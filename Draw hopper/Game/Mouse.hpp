#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

#include <SFML/Graphics.hpp>



class Mouse {
private:
    sf::Cursor cursor;
    sf::Image whiteCursor;
    sf::Image redCursor;
    bool pressed;

public:

    Mouse() {
        whiteCursor.loadFromFile("res/white_cursor.png");
        redCursor.loadFromFile("res/red_cursor.png");
    }

    void setDefault(sf::RenderWindow& windRef) {
        cursor.loadFromPixels(whiteCursor.getPixelsPtr(), sf::Vector2u(6,6), sf::Vector2u(3,3));
        windRef.setMouseCursor(cursor);

        pressed = false;
    }
    void setPressed(sf::RenderWindow& windRef) {
        cursor.loadFromPixels(redCursor.getPixelsPtr(), sf::Vector2u(6,6), sf::Vector2u(3,3));
        windRef.setMouseCursor(cursor);

        pressed = true;
    }
    bool IsPressed() const { return pressed; }
};

#endif // MOUSE_H_INCLUDED
