#include <SFML/Graphics.hpp>
#include "wall.hpp"

Wall::Wall(float x1, float y1, float x2, float y2) {
    p1.x = x1;
    p1.y = y1;
    p2.x = x2;
    p2.y = y2;

    vertices.setPrimitiveType(sf::Lines);
    vertices.resize(2);
    vertices[0].position = p1;
    vertices[1].position = p2;
}

void Wall::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(vertices, states);
}
