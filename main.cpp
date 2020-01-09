#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "arrow.hpp"
#include "wall.hpp"

using namespace std;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

float radToDeg(float rad) {
    return rad * 180 / M_PI;
}

bool lineLineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    float numerator1 = (y1-y3)*(x4-x3) - (x1-x3)*(y4-y3);
    float numerator2 = (y1-y3)*(x2-x1) - (x1-x3)*(y2-y1);
    float denominator = (x2-x1)*(y4-y3) - (y2-y1)*(x4-x3);

    if (denominator == 0) {
        if (numerator1 == 0 && numerator2 == 0) {
            return true;
        }
        else {
            return false;
        }
    }

    float a = numerator1 / denominator;
    float b = numerator2 / denominator;

    if (a >= 0 && a <= 1 && b >= 0 && b <= 1) {
        return true;
    }
    else {
        return false;
    }
}

bool lineRectCollision(float x1, float y1, float x2, float y2, sf::FloatRect rect) {
    sf::Vector2f topLeft(rect.left, rect.top);
    sf::Vector2f bottomLeft(rect.left, rect.top + rect.height);
    sf::Vector2f topRight(rect.left + rect.width, rect.top);
    sf::Vector2f bottomRight(rect.left + rect.width, rect.top + rect.height);

    bool left = lineLineCollision(x1, y1, x2, y2, topLeft.x, topLeft.y, bottomLeft.x, bottomLeft.y);
    bool right = lineLineCollision(x1, y1, x2, y2, topRight.x, topRight.y, bottomRight.x, bottomRight.y);
    bool top = lineLineCollision(x1, y1, x2, y2, topLeft.x, topLeft.y, topRight.x, topRight.y);
    bool bottom = lineLineCollision(x1, y1, x2, y2, bottomLeft.x, bottomLeft.y, bottomRight.x, bottomRight.y);

    if (left || right || top || bottom) {
        return true;
    }
    else {
        return false;
    }
}

bool checkCollision(Arrow arrow, const vector<Wall> &walls, float dx, float dy) {
    arrow.x += dx;
    arrow.y += dy;

    for (auto wall = walls.begin(); wall != walls.end(); wall++) {
        if (lineRectCollision((*wall).p1.x, (*wall).p1.y, (*wall).p2.x, (*wall).p2.y, arrow.collisionBox.getGlobalBounds())) {
            return true;
        }
    }

    return false;
}

void update(Arrow &arrow, const vector<Wall> &edges, const vector<Wall> &walls) {
    float dx = arrow.movementSpeed * cos(degToRad(arrow.a));
    float dy = arrow.movementSpeed * sin(degToRad(arrow.a));

    if (checkCollision(arrow, edges, dx, dy) == true || checkCollision(arrow, walls, dx, dy) == true) {
        cout << "collision" << endl;
        // Figure out collision resolution
    }
    else {
        cout << "no collision" << endl;
    }

    sf::Transform translation;
    translation.translate(dx, dy);

    sf::Transform rotation;
    rotation.rotate(arrow.rotationalSpeed, arrow.x, arrow.y);

    arrow.x += dx;
    arrow.y += dy;
    arrow.a += arrow.rotationalSpeed;

    arrow.x = fmod(arrow.x + 500, 500);
    arrow.y = fmod(arrow.y + 500, 500);
    arrow.a = fmod(arrow.a, 360);

    sf::Transform transform = translation * rotation;
    arrow.vertices[0].position = transform.transformPoint(arrow.vertices[0].position);
    arrow.vertices[1].position = transform.transformPoint(arrow.vertices[1].position);
    arrow.vertices[2].position = transform.transformPoint(arrow.vertices[2].position);
    arrow.collisionBox.setPosition(arrow.x - arrow.width/2, arrow.y - arrow.width/2);
}

int main(){
    sf::RenderWindow window(sf::VideoMode(500, 500), "Raycasting", sf::Style::Resize);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    Arrow arrow(window.getSize().x, window.getSize().y);

    vector<Wall> walls;
    walls.push_back(Wall(100, 100, 400, 100));
    walls.push_back(Wall(400, 100, 400, 400));
    walls.push_back(Wall(400, 400, 100, 400));
    walls.push_back(Wall(100, 400, 100, 100));

    vector<Wall> edges;
    edges.push_back(Wall(0, 0, 500, 0));
    edges.push_back(Wall(500, 0, 500, 500));
    edges.push_back(Wall(500, 500, 0, 500));
    edges.push_back(Wall(0, 500, 0, 0));

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Left:
                        arrow.rotationalSpeed = -2.5;
                        break;
                    case sf::Keyboard::Right:
                        arrow.rotationalSpeed = 2.5;
                        break;
                    case sf::Keyboard::Up:
                        arrow.movementSpeed = 2;
                        break;
                    case sf::Keyboard::Down:
                        arrow.movementSpeed = -2;
                        break;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                switch(event.key.code) {
                    case sf::Keyboard::Left:
                    case sf::Keyboard::Right:
                        arrow.rotationalSpeed = 0;
                        break;
                    case sf::Keyboard::Up:
                    case sf::Keyboard::Down:
                        arrow.movementSpeed = 0;
                        break;
                }
            }
        }

        // arrow.update(walls);
        update(arrow, edges, walls);

        window.clear(sf::Color::Blue);
        for (auto wall = walls.begin(); wall != walls.end(); wall++) {
            window.draw(*wall);
        }

        window.draw(arrow.collisionBox);
        window.draw(arrow.vertices);
        
        window.display();
    }

    return 0;
}