#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    int textureOff = 0;
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

void Replace(sf::Vector2i a[4], sf::Vector2i b[4]) {
    for(int i = 0; i < 4; i++)
        a[i] = b[i];
}

bool Did_land(sf::Vector2i shape[4]) {   /// Tikrina, ar nusileido
    for(int i = 0; i < 4; i++) {
        if(Field[shape[i].y+1][shape[i].x].filled) return true; /// Ar ant kitos detales
        if(shape[i].y == 19) return true;                /// Ar ant pagrindo
    }
    return false;
}

/// Pasirenka atsitiktina forma (Ja uzpildo i "a[4]")
/// Argumentuose persiunciama praeita forma,
/// kad ji nepasikartotu
ShapeData Choose_shape(ShapeData prev_shape) {
    /// Atsitiktinai pasirenkama viena teksturos plytele
    std::srand(std::time(nullptr));
    int textureOff = prev_shape.textureOff;

    while(textureOff == prev_shape.textureOff) textureOff = rand() % 7;


    /// Atsitiktinai pasirenka forma
    int shapeNum = rand() % 7;
    for(int i = 0; i < 4; i++) {
        a[i].x = (Shapes[shapeNum][i] % 2) + 3;  /// Gauna formos kvadratelio starto x verte
        a[i].y = (Shapes[shapeNum][i] / 2);  /// Gauna formos kvadratelio starto y verte
    }
    return {textureOff, shapeNum};
}

int main() {
    sf::RenderWindow window(sf::VideoMode(tileSize * 10, tileSize * 20), "Tetris!", sf::Style::Titlebar | sf::Style::Close);

    /// ------------- GARSAI IR MUZIKA ------------------------------
    sf::SoundBuffer buff1;
    buff1.loadFromFile("rec1.wav");
    sf::Sound sound(buff1);

    sf::Music fono_muzika;
    fono_muzika.openFromFile("theme.ogg");
    fono_muzika.setLoop(true);
    /// ------------ TEKSTAS EKRANE --------------------------------
    sf::Font font;
    font.loadFromFile("Roboto-font.ttf");

    sf::Text text;

    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);

    sf::Text time_text = text, fps_text = text, finished_text = text, score_text = text;

    score_text.setPosition(6.f, 6.f);
    fps_text.setPosition(150.f, 6.f);
    finished_text.setPosition(20.f, 100.f);
    finished_text.setCharacterSize(24);
    ///  ----------- UZKRAUNAMA TEKSTURA ---------------------------
    sf::Texture t;
    t.loadFromFile("tiles.png");
    sf::Sprite s(t);

    /// ----------- KITI KINTAMIEJI ------------------------------
    // Zaidimui
    bool paused = false;      /// Ar sustabdytas zaidimas
    bool finished = false;    /// Ar pasibaige zaidimas
    int score = 0;
    // Detalei
    ShapeData shape;           /// judancios formos duomenys
    int dx = 0;              /// -1 i kaire, +1 i desine
    bool turn = false;       /// Ar pasukti detale
    bool down = false;       /// Ar kristi zemyn
    // Laikui
    float lastTime = 0.f;
    float timer = 0.f;            /// zaidime praejes laikas sekundemis
    float original_period = 1.f;
    float period = original_period;     /// laikas sekundemis, po kurio detale pajuda zemyn
    float fps;              /// Kadrai per sekunde


    shape = Choose_shape({-1, -1}); /// sukuriama pirma detale
    fono_muzika.play();
    /// ------------ ZAIDIMO CIKLAS -------------------------
    sf::Clock clock;
    while (window.isOpen()) {
        /// ------------- LAIKO SKAICIAVIMAI --------------------------
        float currentTime = clock.getElapsedTime().asSeconds();
        timer += currentTime - lastTime;
        fps = std::floor(std::abs(1.f / (currentTime - lastTime)));
        lastTime = currentTime;

        fps_text.setString("fps " + Num_to_string(fps));
        /// ---------- MYGTUKU TIKRINIMAS -----------------------------
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Space) {
                    if(paused) {
                        paused = false;
                        fono_muzika.play();
                    } else {
                        paused = true;
                        fono_muzika.pause();
                    }
                }
                if(event.key.code == sf::Keyboard::M) {
                    if(fono_muzika.getVolume() == 0) fono_muzika.setVolume(100);
                    else                             fono_muzika.setVolume(0);
                }
                if(event.key.code == sf::Keyboard::Left) dx = -1;
                else if(event.key.code == sf::Keyboard::Right) dx = 1;
                if(event.key.code == sf::Keyboard::Up) turn = true;
                else if(event.key.code == sf::Keyboard::Down) down = true;
            }
        }
        if(!finished && !paused){

        /// ----------  DETALES JUDEJIMAS ZEMYN --------------------------------------------------
        if(down && !Did_land(a)) {    /// Kai pats paspaudzia i apacia
            score++;
            for(int i = 0; i < 4; i++)
                b[i].y = a[i].y+1;

            if(Check(b))
                Replace(a, b);
        }

        if(timer > period) {         /// Kai praeina laiko periodas
            if(Did_land(a)) {   /// Uzfiksuojam, jeigu nusileido
                /// Surasom Detales duomenis i laukelius, tolesniam piesimui ir tikrinimui
                for(int i = 0; i < 4; i++) {
                    Field[a[i].y][a[i].x].filled = true;
                    Field[a[i].y][a[i].x].textureOff = shape.textureOff;
                }

                /// ----------- PARENKAM NAUJA FORMA -----------------------------------------
                down = false;
                shape = Choose_shape(shape);

                /// ----------- TIKRINAM AR PASIBAIGE ZAIDIMAS -------------------------------------
                if(!Check(a)) {
                    finished = true;
                }

                timer = 0.f;
                continue;
            }

            for(int i = 0; i < 4; i++)
                b[i].y = a[i].y+1;

            if(Check(b))
                Replace(a, b);

            timer = 0.f;
        }

        /// ------------- EILUCIU UZPILDYMO LOGIKA ----------------------------------------------
        int filled_lines = 0;
        for(int i = 0; i < 20; i++) {
            int suma = 0;

            for(int j = 0; j < 10; j++)
                suma += (int)(Field[i][j].filled);
            if(suma == 10) {
                sound.play();
                period  = std::max(0.15f, period * 0.9f);

                filled_lines++;
                score += (filled_lines == 4)? 800 : 100;  /// Duoda taskus
                for(int eil = i; eil > 0; eil--)
                {
                    for(int j = 0; j < 10; j++) {
                        Field[eil][j].filled = Field[eil-1][j].filled;
                        Field[eil][j].textureOff = Field[eil-1][j].textureOff;
                    }
                }
            }
        }
        filled_lines = 0;


        /// ------- DETALES JUDEJIMAS   Krairen, desinen  ----------------------------------------
        for(int i = 0; i < 4; i++)
            b[i] = {a[i].x + dx, a[i].y};
        if(Check(b)) Replace(a, b);

        /// ------- DETALES SUKIMASIS ------------------------------------------------------------
        if(turn == true && shape.shape_num != 4) {
            sf::Vector2i p = a[1];   /// Taskas, apie kuri suksis detale

            for(int i = 0; i < 4; i++) {
                int x = -(a[i].y - p.y) + p.x;
                int y = (a[i].x - p.x) + p.y;

                b[i] = {x, y};
            }
            if(Check(b)) Replace(a, b);
            /// Jeigu pasisukti neiseina, tai tikrinam, ar detale gali
            /// pasisukti pirma pajudant i sona
            else {
                for(int i = 0; i < 4; i++)       /// Pastumiam i kaire
                    b[i] = {b[i].x -1, b[i].y};
                if(Check(b)) Replace(a, b);         /// ir patikrinam
                else {
                    for(int i = 0; i < 4; i++)       /// Pastumiam i desine
                        b[i] = {b[i].x + 2, b[i].y};
                    if(Check(b))  Replace(a, b);       /// ir patikrinam
                    else {
                        for(int i = 0; i < 4; i++)       /// Pastumiam i desine
                            b[i] = {b[i].x - 1, b[i].y+1};
                        if(Check(b))  Replace(a, b);       /// ir patikrinam
                    }
                }
            }

        }

        /// ------------- PIESIMAS --------------------------------------------------------
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
        score_text.setString("Score: " + Num_to_string(score));
        window.draw(score_text);
        window.draw(fps_text);
        window.display();

        /// ------------ ISVALYMAS ----------------------------------------------------------------
        dx = 0;
        turn = false;
        down = false;

    }  /// if(!finished)
    else if(finished && !paused) {

        finished_text.setString("    THE END.\n Your score is:\n\n\n       " + Num_to_string(score));
        window.draw(finished_text);
        window.display();
    }
    else if(!finished && paused) {
        finished_text.setString("    PAUSED.\n Your score is:\n\n\n       " + Num_to_string(score));
        window.draw(finished_text);
        window.display();
    }
    }

    return 0;
}



