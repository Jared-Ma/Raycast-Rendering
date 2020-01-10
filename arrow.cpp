#include <SFML/Graphics.hpp>
#include <cmath>
#include "arrow.hpp"
#include "helper.hpp"
// using namespace std;

Arrow::Arrow(unsigned int windowWidth, unsigned int windowHeight) {
    x = windowWidth / 2;
    y = windowHeight / 2;
    a = 0;
    movementSpeed = 0;
    rotationalSpeed = 0;
    width = 14;
    height = 20;
    
    for (int i = 0; i < 10; i += 10) {
        rays.push_back(Ray(x, y, toRadians(i)));
    }

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

void Arrow::update(const vector<Wall> &edges, const vector<Wall> &walls) {
    float dx = movementSpeed * cos(toRadians(a));
    float dy = movementSpeed * sin(toRadians(a));

    // if (checkCollision(this, edges, dx, dy) == true || checkCollision(this, walls, dx, dy) == true) {
    //     cout << "collision" << endl;
    //     // Figure out collision resolution
    // }
    // else {
    //     cout << "no collision" << endl;
    // }

    sf::Transform translation;
    translation.translate(dx, dy);

    sf::Transform rotation;
    rotation.rotate(rotationalSpeed, x, y);

    x += dx;
    y += dy;
    a += rotationalSpeed;

    x = fmod(x + 500, 500);
    y = fmod(y + 500, 500);
    a = fmod(a, 360);

    sf::Transform transform = translation * rotation;
    vertices[0].position = transform.transformPoint(vertices[0].position);
    vertices[1].position = transform.transformPoint(vertices[1].position);
    vertices[2].position = transform.transformPoint(vertices[2].position);
    collisionBox.setPosition(x - width/2, y - width/2);
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(vertices, states);
}