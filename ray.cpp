#include <cmath>
#include "ray.hpp"
#include "helper.hpp"

Ray::Ray(float x, float y, float a) {
    this->x = x;
    this->y = y;
    
    direction.x = cos(toRadians(a));
    direction.y = sin(toRadians(a));
}

bool Ray::cast(x1, y1, x2, y2) {
    float x3 = x;
    float y3 = y;
    float x4 = x + direction.x;
    float y4 = y + direction.y;

    float denominator = (x2-x1)*(y4-y3) - (y2-y1)*(x4-x3);

    if (denominator == 0) {
        return false;
    }

    float numerator1 = (y1-y3)*(x4-x3) - (x1-x3)*(y4-y3);
    float numerator2 = (y1-y3)*(x2-x1) - (x1-x3)*(y2-y1);

    float a = numerator1 / denominator;
    float b = numerator2 / denominator;

    if (a > 0 && a < 1 && b > 0) {
        return true;
    }
    else {
        return false;
    }
}

void Ray::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(vertices, states);
}