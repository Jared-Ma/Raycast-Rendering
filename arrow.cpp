#include <SFML/Graphics.hpp>
#include "arrow.hpp"

Arrow::Arrow(unsigned int windowWidth, unsigned int windowHeight) {
    x = windowWidth / 2;
    y = windowHeight / 2;
    a = 0;
    movementSpeed = 0;
    rotationalSpeed = 0;
    width = 14;
    height = 20;
    
    vertices.setPrimitiveType(sf::Triangles);
    vertices.resize(3);
    vertices[0].position = sf::Vector2f(x + height/2, y);
    vertices[1].position = sf::Vector2f(x - height/2, y + width/2);
    vertices[2].position = sf::Vector2f(x - height/2, y - width/2);

    collisionBox.setSize(sf::Vector2f(width, width));
    collisionBox.setPosition(x - width/2, y - width/2);
    collisionBox.setFillColor(sf::Color::Transparent);
    collisionBox.setOutlineThickness(1);
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(vertices, states);
}