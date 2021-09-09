#ifndef VOLUMELIGHT2D_H_INCLUDED
#define VOLUMELIGHT2D_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

sf::ConvexShape createCircle(const float & radius, float x = 0.f, float y = 0.f, int lineSegments = 15);
sf::ConvexShape createRectangle(const float & with, const float & height, float x = 0.f, float y = 0.f);
sf::ConvexShape createSquare(float size, float x = 0.f, float y = 0.f);


template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}

class PointLight : public sf::Drawable {
private:
struct PolyPoint {
    sf::Vector2f point;
    float angle;
    PolyPoint() {};
    PolyPoint(const float & angle, const sf::Vector2f & point)
        : point(point), angle(angle) {}
};


    float *W;
    float *H;

    float radius;
    sf::Vector2f center;
    sf::Vector3f color;
    vector<sf::Vertex> polygon;

    sf::Shader shader;
    sf::RenderStates state;


    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(&polygon[0], polygon.size(), sf::TriangleFan, state);
    }

public:
    PointLight(const sf::Vector2f & pos, const float & radius, const sf::Color & color, float * W, float * H)
    : center(pos), radius(radius), color((float)color.r / 255, (float)color.g / 255, (float)color.b / 255), W(W), H(H) {
        shader.loadFromFile("Light.glsl", sf::Shader::Fragment);
        state.blendMode = sf::BlendAdd;
        state.shader = &shader;
    }
    /// MAKE A COPY CONSTRUCTOR

    PointLight(const PointLight & p){
        this->W = new float;
        *(this->W) = *(p.W);
        this->H = new float;
        *(this->H) = *(p.H);

        this->radius = p.radius;
        this->center = p.center;
        this->color = p.color;
        this->shader.loadFromFile("Light.glsl", sf::Shader::Fragment);
        this->state.blendMode = sf::BlendAdd;
        this->state.shader = &(this->shader);
    }


    ~PointLight() {
        delete W;
        delete H;
    }
    void calculateVisibilityPolygon(vector<sf::ConvexShape> & shapes);
    void setSrcPosition(const sf::Vector2f & pos) {
        this->center = pos;
    }
    void setSrcPosition(const sf::Vector2i & pos) {
        this->center = sf::Vector2f(pos);
    }
    sf::Vector2f getSrcPosition() const {
        return this->center;
    }
    void setRadius(const float & radius) {
        this->radius = clip(radius, 0.001f, *W * 1.25f);
    }
    float getRadius() const { return this->radius; }
    void setColor(const sf::Color & color) {
        this->color = sf::Vector3f((float)color.r / 255, (float)color.g / 255, (float)color.b / 255);
    }
    sf::Color getColor() const {
        return sf::Color(color.x * 255, color.y * 255, color.z * 255);
    }
    void setWindowPointers(float * W, float * H) {
        this->W = W;
        this->H = H;
    }

    void update() {
        sf::Vector2f Pos(center);
        Pos.y = ((*H * 0.5f) - Pos.y) + (*H * 0.5f);
        shader.setUniform("u_srcPos", Pos);
        shader.setUniform("u_lightColor", color);
        shader.setUniform("u_lightRadius", radius);
    }

};


void PointLight::calculateVisibilityPolygon(vector<sf::ConvexShape> & shapes) {
    //shapes.push_back(createSquare(radius * 2, center.x - radius, center.y - radius));
    sf::ConvexShape BigBox(createRectangle(*W, *H));
    vector<sf::ConvexShape> goodShapes;
    goodShapes.push_back(BigBox);
    for(sf::ConvexShape & shape : shapes) {
        float xOff = shape.getPosition().x - shape.getOrigin().x;
        float yOff = shape.getPosition().y - shape.getOrigin().y;

        for(int i = 0; i < shape.getPointCount(); i++) {
            float x = shape.getPoint(i).x + xOff;
            float y = shape.getPoint(i).y + yOff;

            float distance = std::hypot(center.x-x, center.y-y);
            if(distance < radius * 0.8f) {
                goodShapes.push_back(shape);
                break;
            }
        }

    }


    polygon.clear();
    vector<PolyPoint> points;

    for(auto &shape : goodShapes) {
        float xOff = shape.getPosition().x - shape.getOrigin().x;
        float yOff = shape.getPosition().y - shape.getOrigin().y;

        for(int i = 0; i < shape.getPointCount(); i++) {
            float x = shape.getPoint(i).x + xOff;
            float y = shape.getPoint(i).y + yOff;

            sf::Vector2f ray(x-center.x, y-center.y);

            float base_angle = std::atan2(ray.y, ray.x);
            float angle = 0.f;
            for(int j = 0; j < 3; j++) {
                if(j == 0) angle = base_angle - 0.00001f;
                if(j == 1) angle = base_angle;
                if(j == 2) angle = base_angle + 0.00001f;

                /// Ray that shoots to the point of length 'radius'
                ray.x = radius * std::cos(angle);
                ray.y = radius * std::sin(angle);


                /// FUNCTION HERE
                float minDist = 1000000000.f;
                PolyPoint closestPoint;
                bool valid = false;

                /// get intersection with an edge
                for(auto &shape2 : goodShapes) {
                    float xOff2 = shape2.getPosition().x - shape2.getOrigin().x;
                    float yOff2 = shape2.getPosition().y - shape2.getOrigin().y;
                    for(int k = 0; k < shape2.getPointCount(); k++) {
                        /// get an Edge from the shape
                        /// Start of edge
                        float e_sX = shape2.getPoint(k).x + xOff2;
                        float e_sY = shape2.getPoint(k).y + yOff2;
                        /// End of edge
                        float kk = (k == shape2.getPointCount()-1)? 0 : k+1;
                        //float kk = k+1;

                        float e_eX = shape2.getPoint(kk).x + xOff2;
                        float e_eY = shape2.getPoint(kk).y + yOff2;

                        /// get an edge as a Vector
                        float vX = e_eX - e_sX;
                        float vY = e_eY - e_sY;

                        /// Check if the lines are not parallel
                        if(fabs(vX - ray.x) > 0.0f && fabs(vY - ray.y) > 0.0f) {
                            float edgeDist = (ray.x * (e_sY - center.y) + (ray.y * (center.x - e_sX))) / (vX * ray.y - vY * ray.x);
                            float rayDist = (e_sX + vX * edgeDist - center.x) / ray.x;

                            /// Check if ray reaches the edge
                            if(rayDist > 0.f && edgeDist >= 0.f && edgeDist <= 1.f) {

                                /// check if the intersection is the closest one
                                if(rayDist < minDist) {
                                    valid = true;
                                    minDist = rayDist;
                                    float min_x = center.x + ray.x * rayDist;
                                    float min_y = center.y + ray.y * rayDist;

                                    closestPoint = PolyPoint(
                                                       std::atan2(min_y - center.y, min_x - center.x),
                                                       sf::Vector2f(min_x, min_y)
                                                   );
                                }
                            }
                        }

                    }
                }
                if(valid)
                    points.push_back(closestPoint);
            }
        }
    }

    //for(auto ptr : goodShapes)
    //    delete ptr;



    /// Sorting the points based on the angle
    std::sort(
        points.begin(),
        points.end(),
    [&](const PolyPoint & p1, const PolyPoint & p2) {
        return p1.angle < p2.angle;
    });
    /// Removing very similar points
    auto it = std::unique(
        points.begin(),
        points.end(),
        [&](const PolyPoint & p1, const PolyPoint & p2) {
            return (abs(p1.point.x - p2.point.x) < 0.1f && abs(p1.point.y - p2.point.y) < 0.1f);
        }
    );
    points.resize(std::distance(points.begin(), it));

    polygon.push_back(sf::Vertex(center, sf::Color::White));
    for(int i = 0; i < points.size(); i++)
        polygon.push_back(sf::Vertex(points[i].point));

    polygon.push_back(sf::Vertex(points[0].point));
}

sf::ConvexShape createCircle(const float & radius, float x, float y, int lineSegments) {
    sf::ConvexShape circle(lineSegments);
    const float PI = 3.14159265f;

    for(int i = 0; i < lineSegments; i++) {
        float angle = ((float)i / (float)lineSegments)  * (2 * PI);
        float xx = std::cos(angle) * radius + radius;
        float yy = std::sin(angle) * radius + radius;
        circle.setPoint(i, {xx, yy});
    }
    circle.setFillColor(sf::Color::White);
    circle.setPosition(x, y);

    return circle;
}

sf::ConvexShape createRectangle(const float & width, const float & height, float x, float y) {
    sf::ConvexShape rect(4);
    rect.setPoint(0, {0.f, 0.f});
    rect.setPoint(1, {width, 0});
    rect.setPoint(2, {width, height});
    rect.setPoint(3, {0, height});
    rect.setFillColor(sf::Color::White);
    rect.setPosition(x, y);

    return rect;
}
sf::ConvexShape createSquare(float size, float x, float y) {
    return createRectangle(size, size, x, y);
}


#endif // VOLUMELIGHT2D_H_INCLUDED
