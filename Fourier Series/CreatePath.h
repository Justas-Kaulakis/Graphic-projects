#ifndef CREATEPATH_H_INCLUDED
#define CREATEPATH_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

class CreatePath : public sf::Drawable {
private:
    vector<sf::Vector2f> points;
    bool stop;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const {
        if(stop) return;
        int N = points.size();

        sf::VertexArray line(sf::LineStrip, N);
        for(int i = 0; i < N; i++)
            line[i].position = points[i];

        target.draw(line);
    }

public:
    CreatePath() : stop(false) {}
    void addPoint(const sf::Vector2f & p) {
        points.push_back(p);
    }
    void addPoint(const sf::Vector2i & p) {
        points.push_back(sf::Vector2f(p));
    }
    vector<sf::Vector2f> getPath() const {
        return points;
    }
    void stopDraw() {
        stop = true;
    }
};

#endif // CREATEPATH_H_INCLUDED
