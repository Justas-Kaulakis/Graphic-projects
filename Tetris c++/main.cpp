#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>    /// Skaito ir raso i stringstream objekta. Is jo gali gauti std::string
#include <algorithm>
#include <time.h>
#include <string>
#include <cmath>


///  Apacioj (Shapes) laikau kvadratelius, is kuriu susidaro kiekviena forma.
///  0 1  . .
///  2 3  . x
///  4 5  x x   Cia "T" forma susidaro is 3,4,5,7 kvadratelio
///  6 7  . x
///

const int tileSize = 26; /// Plyteles dydis

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
std::string Num_to_string(T &num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}


int main()
{
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



    /// Atsitiktinai pasirenkama viena teksturos plytele
    std::srand(std::time(nullptr));
    int tile = rand() % 8;
    s.setTextureRect(sf::IntRect(tile * tileSize, 0, tileSize, tileSize));



    int dx = 0;
    bool turn = false;
    // Laikas
    float lastTime = 0.f;
    float timer = 0.f;            /// zaidime praejes laikas sekundemis
    float period = 1.f;     /// laikas sekundemis, po kurio detale pajuda zemyn
    float fps;              /// Kadrai per sekunde

    /// Suskaiciuoju atsitiktinos formos (x,y) taskus
    //int n = rand() % 7;
    int n = 4;
    for(int i = 0; i < 4; i++){
        a[i].x = Shapes[n][i] % 2;  /// Gauna x verte
        a[i].y = Shapes[n][i] / 2;  /// Gauna y verte
    }


    /// ------------ ZAIDIMO CIKLAS -------------------------
    sf::Clock clock;
    sf::Clock fps_clock;
    while (window.isOpen())
    {
        /// ------------- LAIKO SKAICIAVIMAI --------------------------
        float time = clock.restart().asSeconds();
        timer += time;

        float currentTime = fps_clock.getElapsedTime().asSeconds();
        fps = std::floor(std::abs(1.f / (currentTime - lastTime)));
        lastTime = currentTime;

        time_text.setString(Num_to_string(timer));
        fps_text.setString(Num_to_string(fps));
        /// ---------- MYGTUKU TIKRINIMAS -----------------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Left) dx = -1;
                else if(event.key.code == sf::Keyboard::Right) dx = 1;
                if(event.key.code == sf::Keyboard::Up) turn = true;
            }
        }

        /// ----------  DETALES JUDEJIMSA ZEMYN --------------------------------------------------
        if(timer > period){
            for(int i = 0; i < 4; i++) a[i].y++;
            timer = 0.f;
        }

        /// ------- DETALES JUDEJIMAS   Krairen, desinen  ----------------------------------------
        for(int i = 0; i < 4; i++) a[i].x += dx;

        /// ------- DETALES SUKIMASIS ------------------------------------------------------------
        if(turn == true && n != 4) {
            sf::Vector2i p = a[1];   /// Taskas, apie kuri suksis detale

            for(int i = 0; i < 4; i++) {
                int x = -(a[i].y - p.y) + p.x;
                int y = (a[i].x - p.x) + p.y;

                a[i] = {x, y};
            }
        }

        for(long int i = 0; i < 10000000; i++);

        /// PIESIMAS
        window.clear(sf::Color::White);
        for(int i = 0; i < 4; i++) {
            s.setPosition((float)(a[i].x * tileSize), (float)(a[i].y * tileSize));
            window.draw(s);
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
