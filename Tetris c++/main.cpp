#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>


int Shapes[7][4] = {
    1,3,5,7,  /// I
    3,4,5,6,  /// Z
    2,4,5,7,  /// S
    3,5,4,7,  /// T
    2,3,4,5,  /// O
    2,5,3,7,  /// L
    3,5,6,7   /// J
};

sf::Vector2i a[4], b[4];


int main()
{
    sf::RenderWindow window(sf::VideoMode(26 * 10, 26 * 20), "Tetris!");

    ///  ----------- TEKSTURA ---------------------------
    sf::Texture t;
    t.loadFromFile("tiles.png");
    sf::Sprite s(t);

    int tile = 0;
    s.setTextureRect(sf::IntRect(tile * 26, 0, 26, 26));

    /// ------------ ZAIDIMO CIKLAS -------------------------

    while (window.isOpen())
    {
        /// ---------- MYGTUKU TIKRINIMAS -----------------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }





        /// PIESIMAS
        window.clear(sf::Color::White);
        window.draw(s);
        window.display();
    }

    return 0;
}
