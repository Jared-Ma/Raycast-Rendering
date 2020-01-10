#include <SFML/Graphics.hpp>
#include <cmath>
#include "ray.hpp"
#include "helper.hpp"

Ray::Ray(float x, float y, float a) {
    this->x = x;
    this->y = y;
    this->a = a;
    
    direction = sf::Vector2f(cos(toRadians(a)), sin(toRadians(a)));

    endpoint = sf::Vector2f(NAN, NAN);

    vertices.setPrimitiveType(sf::Lines);
    vertices.resize(2);
    vertices[0].position = sf::Vector2f(x, y);
    vertices[1].position = sf::Vector2f(NAN, NAN);
}

sf::Vector2f Ray::cast(float x1, float y1, float x2, float y2) {
    float x3 = x;
    float y3 = y;
    float x4 = x + direction.x;
    float y4 = y + direction.y;

    sf::Vector2f pt(NAN, NAN);

    float denominator = (x2-x1)*(y4-y3) - (y2-y1)*(x4-x3);

    if (denominator == 0) {
        return pt;
    }

    float numerator1 = (y1-y3)*(x4-x3) - (x1-x3)*(y4-y3);
    float numerator2 = (y1-y3)*(x2-x1) - (x1-x3)*(y2-y1);

    float a = numerator1 / denominator;
    float b = numerator2 / denominator;

    if (a >= 0 && a <= 1 && b >= 0) {
        pt.x = x1 + a * (x2-x1);
        pt.y = y1 + a * (y2-y1);
    }

    return pt;
}

void Ray::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(vertices, states);
}