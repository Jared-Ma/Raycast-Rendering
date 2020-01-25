#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "arrow.hpp"
#include "helper.hpp"
using namespace std;

Arrow::Arrow(unsigned int windowWidth, unsigned int windowHeight) {
    x = windowWidth / 2;
    y = windowHeight / 2;
    a = 0;
    movementSpeed = 0;
    rotationalSpeed = 0;
    width = 14;
    height = 20;
    fov = 100;
    
    for (float i = -fov/2; i <= fov/2; i += 0.5) {
        rays.push_back(Ray(x, y, i));
    }

    vertices.setPrimitiveType(sf::Triangles);
    vertices.resize(3);
    vertices[0].position = sf::Vector2f(x + height/2, y);
    vertices[1].position = sf::Vector2f(x - height/2, y + width/2);
    vertices[2].position = sf::Vector2f(x - height/2, y - width/2);

    // collisionBox.setSize(sf::Vector2f(width, width));
    // collisionBox.setPosition(x - width/2, y - width/2);
    // collisionBox.setFillColor(sf::Color::Transparent);
    // collisionBox.setOutlineThickness(1);
}

void Arrow::update(const vector<Wall> &edges, const vector<Wall> &walls) {
    float dx = movementSpeed * cos(toRadians(a));
    float dy = movementSpeed * sin(toRadians(a));

    sf::Transform translation;
    translation.translate(dx, dy);

    sf::Transform rotation;
    rotation.rotate(rotationalSpeed, x, y);

    sf::Transform transform = translation * rotation;
    vertices[0].position = transform.transformPoint(vertices[0].position);
    vertices[1].position = transform.transformPoint(vertices[1].position);
    vertices[2].position = transform.transformPoint(vertices[2].position);
    // collisionBox.setPosition(x - width/2, y - width/2);

    x += dx;
    y += dy;
    a += rotationalSpeed;

    // Update rays
    for (int i = 0; i < rays.size(); i++) {
        rays[i].x = x;
        rays[i].y = y;
        rays[i].a += rotationalSpeed;
        rays[i].direction = sf::Vector2f(cos(toRadians(rays[i].a)), sin(toRadians(rays[i].a)));
        rays[i].endpoint = sf::Vector2f(NAN, NAN);
        rays[i].vertices[0].position = sf::Vector2f(x, y);
        rays[i].vertices[1].position = sf::Vector2f(NAN, NAN);

        float minDistance = INFINITY;

        for (int j = 0; j < walls.size(); j++) {

            sf::Vector2f pt = rays[i].cast(walls[j].p1.x, walls[j].p1.y, walls[j].p2.x, walls[j].p2.y);

            if (!isnan(pt.x) && !isnan(pt.y)) {

                float distance = calcDistance(rays[i].x, rays[i].y, pt.x, pt.y);

                if (distance < minDistance) {
                    minDistance = distance;
                    rays[i].endpoint = pt;
                    rays[i].vertices[1].position = pt;
                }
            }
        }
        rays[i].distance = minDistance;
    }
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(vertices, states);
}