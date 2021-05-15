#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>    /// Skaito ir raso i stringstream objekta. Is jo gali gauti std::string
#include <algorithm>
#include <time.h>
#include <string>
#include <cmath>

struct ShapeData {
    int textureOff;
    int shape_num;
};
struct FieldTile {
    int textureOff;
    bool filled = false;
};

FieldTile Field[20][10]; // Visi laukeliai false

///  Apacioj (Shapes) laikau kvadratelius, is kuriu susidaro kiekviena forma.
///  0 1  |. .|
///  2 3  |. x|
///  4 5  |x x|   Cia "T" forma susidaro is 3,4,5,7 kvadratelio
///  6 7  |. x|

const float tileSize = 26.f; /// Plyteles dydis

int Shapes[7][4] = {
    1,3,5,7,  /// I
    3,4,5,6,  /// Z
    2,5,4,7,  /// S
    3,5,4,7,  /// T
    2,3,4,5,  /// O
    2,5,3,7,  /// L
    3,5,6,7   /// J
};

/// Cia yra tas pats kaip struct taskas{int x, y};
sf::Vector2i a[4], b[4];    /// Cia laikysiu judancios formos visu 4-iu kvadrateliu (x,y) taskus


template<typename T>
std::string Num_to_string(T &num) {     /// Skaiciu pavercia i String
    std::stringstream ss;
    ss << num;
    return ss.str();
}


bool Check(sf::Vector2i shape[4]) {
    for(int i = 0; i < 4; i++) {
        if(shape[i].x < 0 || shape[i].x >= 10) return false;  /// tikrina sienas
        if(shape[i].y < 0 || shape[i].y >= 20) return false;  /// tikrina pagrinda
        if(Field[shape[i].y][shape[i].x].filled)      return false;  /// tikrina kitas detales
    }
    return true;
}

bool Did_land(sf::Vector2i shape[4]) {   /// Tikrina, ar nusileido
    for(int i = 0; i < 4; i++) {
        if(Field[shape[i].y+1][shape[i].x].filled) return true; /// Ar ant kitos detales
        if(shape[i].y == 19) return true;                /// Ar ant pagrindo
    }
    return false;
}
/// Pasirenka atsitiktina forma
/// Argumentuose persiunciama praeita forma,
/// kad ji nepasikartotu
ShapeData Choose_shape(ShapeData prev_shape) {
    /// Atsitiktinai pasirenkama viena teksturos plytele
    std::srand(std::time(nullptr));
    int textureOff = prev_shape.textureOff;
    int shapeNum = rand() % 7;

    while(textureOff == prev_shape.textureOff) textureOff = rand() % 8;

    for(int i = 0; i < 4; i++) {
        a[i].x = (Shapes[shapeNum][i] % 2) + 3;  /// Gauna formos kvadratelio starto x verte
        a[i].y = (Shapes[shapeNum][i] / 2) + 1;  /// Gauna formos kvadratelio starto y verte
    }
    return {textureOff, shapeNum};
}
int main() {
    sf::RenderWindow window(sf::VideoMode(tileSize * 10, tileSize * 20), "Tetris!");

    /// ------------ TEKSTAS EKRANE --------------------------------
    sf::Font font;
    font.loadFromFile("Roboto-font.ttf");

    sf::Text text;

    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);

    sf::Text time_text = text, fps_text = text;

    time_text.setPosition(16.f, 6.f);
    fps_text.setPosition(150.f, 6.f);

    ///  ----------- UZKRAUNAMA TEKSTURA ---------------------------
    sf::Texture t;
    t.loadFromFile("tiles.png");
    sf::Sprite s(t);

    ShapeData shape;           /// judancios formos duomenys
    int dx = 0;              /// -1 i kaire, +1 i desine
    bool turn = false;       /// Ar pasukti detale
    // Laikas
    float lastTime = 0.f;
    float timer = 0.f;            /// zaidime praejes laikas sekundemis
    float period = 0.3f;     /// laikas sekundemis, po kurio detale pajuda zemyn
    float fps;              /// Kadrai per sekunde

    shape = Choose_shape({-1, -1}); /// sukuriama pirma detale

    /// ------------ ZAIDIMO CIKLAS -------------------------
    sf::Clock clock;
    while (window.isOpen()) {
        /// ------------- LAIKO SKAICIAVIMAI --------------------------
        float currentTime = clock.getElapsedTime().asSeconds();
        timer += currentTime - lastTime;
        fps = std::floor(std::abs(1.f / (currentTime - lastTime)));
        lastTime = currentTime;

        time_text.setString(Num_to_string(timer));
        fps_text.setString(Num_to_string(fps));
        /// ---------- MYGTUKU TIKRINIMAS -----------------------------
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Left) dx = -1;
                else if(event.key.code == sf::Keyboard::Right) dx = 1;
                if(event.key.code == sf::Keyboard::Up) turn = true;
            }
        }

        /// ----------  DETALES JUDEJIMAS ZEMYN --------------------------------------------------
        if(timer > period) {
            if(Did_land(a)) {   /// Uzfiksuojam, jeigu nusileido
                /// Surasom Detales duomenis i laukelius, tolesniam piesimui ir tikrinimui
                for(int i = 0; i < 4; i++) {
                    Field[a[i].y][a[i].x].filled = true;
                    Field[a[i].y][a[i].x].textureOff = shape.textureOff;
                }

                shape = Choose_shape(shape); /// Parenkam nauja forma
                //landed = false;

                timer = 0.f;
                continue;
            }

            for(int i = 0; i < 4; i++)
                b[i].y = a[i].y+1;

            if(Check(b))
                for(int i = 0; i < 4; i++)
                    a[i] = b[i];

            timer = 0.f;
        }

        /// ------- DETALES JUDEJIMAS   Krairen, desinen  ----------------------------------------
        for(int i = 0; i < 4; i++)
            b[i] = {a[i].x + dx, a[i].y};
        if(Check(b))
            for(int i = 0; i < 4; i++)
                a[i] = b[i];

        /// ------- DETALES SUKIMASIS ------------------------------------------------------------
        if(turn == true && shape.shape_num != 4) {
            sf::Vector2i p = a[1];   /// Taskas, apie kuri suksis detale

            for(int i = 0; i < 4; i++) {
                int x = -(a[i].y - p.y) + p.x;
                int y = (a[i].x - p.x) + p.y;

                b[i] = {x, y};
            }
            if(Check(b))
                for(int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        /// PIESIMAS
        window.clear(sf::Color::White);
        for(int i = 0; i < 4; i++) {
            s.setPosition((float)(a[i].x * tileSize), (float)(a[i].y * tileSize));
            s.setTextureRect(sf::IntRect(shape.textureOff * tileSize, 0, tileSize, tileSize));
            window.draw(s);
        }
        for(int i = 0; i < 20; i++) {
            for(int j = 0; j < 10; j++) {
                if(Field[i][j].filled) {
                    s.setPosition((float)j * tileSize, (float)i * tileSize);
                    s.setTextureRect(sf::IntRect(Field[i][j].textureOff * tileSize, 0, tileSize, tileSize));
                    window.draw(s);
                }
            }
        }
        window.draw(time_text);
        window.draw(fps_text);
        window.display();

        /// ISVALYMAS
        dx = 0;
        turn = false;
    }

    return 0;
}
