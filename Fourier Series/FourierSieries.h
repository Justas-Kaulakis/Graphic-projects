#ifndef FOURIERSIERIES_H_INCLUDED
#define FOURIERSIERIES_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#define PI 3.141592653f

using namespace std;

struct FSPoint {
    float re, im;
    int freq;
    FSPoint(){};
    FSPoint(const float & re, const float & im, const int & freq)
    : re(re), im(im), freq(freq) {}
    float getAmplitude() const  { return std::hypot(re, im); }
    float getPhase() const { return std::atan2(im, re); }
};
bool compare(const FSPoint & left, const FSPoint & right) {
    return left.getAmplitude() > right.getAmplitude();
}


class FourierSeries {
private:
    vector<FSPoint> output;
    list<sf::Vector2f> signal;
    float time;
    float dt;
    float scale;
    bool start;

    vector<FSPoint> dft(const vector<float> & x, bool turn = false) {
        int N = x.size();
        vector<FSPoint> X(N);

        for(int k = 0; k < N; k++) {
            float re = 0.f;
            float im = 0.f;
            for(int n = 0; n < N; n++) {
                float phi = (2 * PI * k * n) / N;
                re += x[n] * cos(phi);
                im -= x[n] * sin(phi);
            }
            re /= N;
            im /= N;

            if(turn) /// Turn 90 degrees to the left
                X[k] = FSPoint(-im, re, k);
            else
                X[k] = FSPoint(re, im, k);
        }
        return X;
    }
    vector<FSPoint> combine(const vector<FSPoint> & X, const vector<FSPoint> & Y) {
        vector<FSPoint> Big(X.size() + Y.size());
        for(int i = 0; i < X.size(); i++)
            Big[i] = X[i];
        int Xsize = X.size();
        for(int i = 0; i < Y.size(); i++) {
            Big[Xsize + i] = Y[i];
        }
        return Big;
    }
    sf::Vector2f drawCircle(sf::RenderWindow & window, const float & alpha,
            const sf::Vector2f & center, const FSPoint & p, const float & time)
    {
        float radius = p.getAmplitude() * scale;
        float phi = p.freq * time + p.getPhase();
        sf::Vector2f arrow(
            center.x + radius * cos(phi),
            center.y + radius * sin(phi)
        );
        sf::Vertex edge[2] = {
            {center, sf::Color(255, 255, 255, alpha * 255.f)},
            {arrow,  sf::Color(255, 255, 255, alpha * 255.f)}
        };

        sf::CircleShape c(radius);
        c.setOrigin(radius, radius);
        c.setPosition(center);
        c.setFillColor(sf::Color::Transparent);
        c.setOutlineThickness(1.f);
        c.setOutlineColor(sf::Color(255, 255, 255, (int)(alpha * 255.f)));


        window.draw(c);
        window.draw(edge, 2, sf::Lines);
        return arrow;
    }

    void drawSignal(sf::RenderWindow & window, const float & x, const float & y) {
        signal.push_front({x, y});
        int N = signal.size();
        if(N > output.size()) signal.pop_back();

        sf::VertexArray line(sf::LineStrip, N);
        int i = 0;
        for(sf::Vector2f &value : signal) {
            line[i].position = value;
            float a = (((float)N - (float)i) / N) * 255.f;
            line[i].color = sf::Color(a, a, a, 255);
            i++;
        }
        window.draw(line);
    }

public:
    FourierSeries()
    : scale(1.f), time(0.f), start(false)
    { }
    FourierSeries(const vector<float> & input)
    : scale(1.f), time(0.f), dt(2 * PI / input.size()), start(false)
    {
        output = dft(input);
        std::sort(
            output.begin(),
            output.end(),
            compare
        );
    }
    FourierSeries(string  file)
    : scale(1.f), time(0.f), start(true)
    {
        ifstream fd(file);
        vector<float> inX, inY;
        float x, y;
        while(fd >> x >> y) {
            inX.push_back(x);
            inY.push_back(y);
        }
        fd.close();

        output = combine(dft(inX), dft(inY, true));
        std::sort(
            output.begin(),
            output.end(),
            compare
        );
        dt = 2 * PI / inX.size();
    }
    setPath(vector<sf::Vector2f> input) {
        vector<float> inX(input.size()), inY(input.size());
        for(int i = 0; i < input.size(); i++) {
            cout << input[i].x << " " << input[i].y << endl;
            inX[i] = input[i].x;
            inY[i] = input[i].y;
        }

        output = combine(dft(inX), dft(inY, true));
        std::sort(
            output.begin(),
            output.end(),
            compare
        );
        dt = 2 * PI / input.size();
        cout << input.size() << endl;
    }
    void draw(sf::RenderWindow & window) {
        if(!start) return;
        sf::Vector2f prevPos(0.f, 0.f);
        for(int i = 0; i < output.size(); i++) {
            prevPos = drawCircle(window, 0.6f, prevPos, output[i], time);
        }
        sf::CircleShape dot(4.f);
        dot.setFillColor(sf::Color::Red);
        dot.setOrigin(4.f, 4.f);
        dot.setPosition(prevPos);
        window.draw(dot);
        drawSignal(window, prevPos.x, prevPos.y);

        time += dt;
    }
    void setScale(const float & scale) {
        this->scale = scale;
        signal.clear();
    }
    void startDraw() {
        start = true;
    }
};


#endif // FOURIERSIERIES_H_INCLUDED
