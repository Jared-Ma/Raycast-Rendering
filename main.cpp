#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "arrow.hpp"
#include "wall.hpp"
#include "ray.hpp"
#include "helper.hpp"

using namespace std;

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

int main() {
    sf::View mainView(sf::FloatRect(0.f, 0.f, 1260.f, 540.f));
    sf::View rayView(sf::FloatRect(0.f, 0.f, 540.f, 540.f));
    sf::View renderView(sf::FloatRect(0.f, 0.f, 760.f, 540.f));
    
    mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    rayView.setViewport(sf::FloatRect(0.f, 0.f, 3/7.f, 1.f));
    renderView.setViewport(sf::FloatRect(3/7.f, 0.f, 4/7.f, 1.f));

    sf::RenderWindow window(sf::VideoMode(1260, 540), "Raycasting", sf::Style::Resize);
    window.setView(rayView);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape border(sf::Vector2f(1260-8, 540-8));
    border.setPosition(4, 4);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(255, 255, 255, 175));
    border.setOutlineThickness(4);

    Arrow arrow(rayView.getSize().x, rayView.getSize().y);

    vector<Wall> walls;
    walls.push_back(Wall(50, 50, 450, 50));
    walls.push_back(Wall(450, 50, 450, 450));
    walls.push_back(Wall(450, 450, 50, 450));
    walls.push_back(Wall(50, 450, 50, 50));
    walls.push_back(Wall(100, 100, 400, 100));
    walls.push_back(Wall(400, 100, 400, 400));
    walls.push_back(Wall(400, 400, 100, 400));
    walls.push_back(Wall(100, 400, 100, 100));

    vector<Wall> edges;
    edges.push_back(Wall(0, 0, 540, 0));
    edges.push_back(Wall(540, 0, 540, 540));
    edges.push_back(Wall(540, 540, 0, 540));
    edges.push_back(Wall(0, 540, 0, 0));

    sf::Vector2f clickPoint(NAN, NAN);
    sf::Vector2f releasePoint(NAN, NAN);

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
                    case sf::Keyboard::Space:
                        if (!walls.empty()) {
                            walls.pop_back();
                        }
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

            if (event.type == sf::Event::MouseButtonPressed) {
                clickPoint = sf::Vector2f(sf::Mouse::getPosition(window));
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                releasePoint = sf::Vector2f(sf::Mouse::getPosition(window));
                walls.push_back(Wall(clickPoint.x, clickPoint.y, releasePoint.x, releasePoint.y));

                releasePoint = sf::Vector2f(NAN, NAN);
                clickPoint = sf::Vector2f(NAN, NAN);
            }
        }

        arrow.update(edges, walls);
        window.clear(sf::Color::Black);
        window.setView(mainView);
        window.draw(border);
        window.setView(rayView);
        
        for (int i = 0; i < walls.size(); i++) {
            window.draw(walls[i]);
        }

        for (int i = 0; i < arrow.rays.size(); i++) {
            if (!isnan(arrow.rays[i].endpoint.x) && !isnan(arrow.rays[i].endpoint.y)) {
                window.draw(arrow.rays[i]);

                window.setView(renderView);

                float width = renderView.getSize().x / arrow.rays.size();
                float height = 8000 / (cos(toRadians(arrow.rays[i].a - arrow.a)) * arrow.rays[i].distance);

                // cout << "angle: " << arrow.rays[i].a - arrow.a << endl;
                // cout << "distance: " << arrow.rays[i].distance << endl;
                // cout << "width: " << width << endl;
                // cout << "height: " << height << endl;

                sf::RectangleShape wallSegment(sf::Vector2f(width, height));
                wallSegment.setPosition(i*width, 270 - height/2);
                // wallSegment.setFillColor(sf::Color(255, 255, 255, 255));
                wallSegment.setFillColor(sf::Color(255, 255, 255, 150*(255 - arrow.rays[i].distance)/540 + 175));

                window.draw(wallSegment);

                window.setView(rayView);
            }
        }

        // window.draw(arrow.collisionBox);
        sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::CircleShape cursor(2);
        cursor.setPosition(mousePosition.x - cursor.getRadius(), mousePosition.y - cursor.getRadius());

        window.draw(cursor);

        window.draw(arrow.vertices);

        if (!isnan(clickPoint.x) && !isnan(clickPoint.y)) {
            sf::VertexArray newWall(sf::Lines, 2);
            newWall[0].position = sf::Vector2f(clickPoint);
            newWall[1].position = sf::Vector2f(sf::Mouse::getPosition(window));
            // newWall[1].position = mousePosition;
            window.draw(newWall);
        }
        
        window.display();
    }

    return 0;
}