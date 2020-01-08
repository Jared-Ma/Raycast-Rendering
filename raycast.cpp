#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

float radToDeg(float rad) {
    return rad * 180 / M_PI;
}

class Arrow : public sf::Drawable, public sf::Transformable {
    private:
        sf::VertexArray vertices;
        void draw(sf::RenderTarget &target, sf::RenderStates states) const {
            states.transform *= getTransform();
            target.draw(vertices, states);
        }

    public:
        float x;
        float y;
        float a;
        float movementSpeed;
        float rotationalSpeed;

        // float dx;       // change in x (speed)
        // float dy;       // change in y (speed)
        // float da;       // change in rotation
        // float acceleration;
        // float maxSpeed;
        
        Arrow(unsigned int width, unsigned int height) {
            x = width / 2;
            y = height / 2;
            a = 0;
            movementSpeed = 0;
            rotationalSpeed = 0;

            // dx = 0;
            // dy = 0;
            // da = 0;
            // acceleration = 0.1;
            // maxSpeed = 1;
            
            vertices.setPrimitiveType(sf::Triangles);
            vertices.resize(3);
            vertices[0].position = sf::Vector2f(x + 25, y);
            vertices[1].position = sf::Vector2f(x - 25, y + 20);
            vertices[2].position = sf::Vector2f(x - 25, y - 20);
        }

        void update() {

            x += movementSpeed * cos(degToRad(a));
            y += movementSpeed * sin(degToRad(a));
            a += rotationalSpeed;

            // x += dx;
            // y += dy;
            // a += da;

            // dx += moving * acceleration * cos(degToRad(a));
            // cout << "dx: " << dx << endl;
            // dy += moving * acceleration * sin(degToRad(a));
            // cout << "dy: " << dx << endl;

            // if (dx >= 0) {
            //     // dx = min(dx, maxSpeed * cos(degToRad(a)));
            //     dx = min(dx, maxSpeed);
            // }
            // else if (dx < 0) {
            //     // dx = max(dx, -maxSpeed * cos(degToRad(180 + a)));
            //     dx = max(dx, -maxSpeed);
            // }

            // if (dy >= 0) {
            //     // dy = min(dy, maxSpeed * sin(degToRad(a)));
            //     dy = min(dy, maxSpeed);
            // }
            // else if (dy < 0) {
            //     // dy = max(dy, -maxSpeed * sin(degToRad(180 + a)));
            //     dy = max(dy, -maxSpeed);
            // }

            x = fmod(x + 500, 500);
            y = fmod(y + 500, 500);
            a = fmod(a, 360);
        }
};

// class Ray {

// }

// class Wall {
    
// }



int main(){
    sf::RenderWindow window(sf::VideoMode(500, 500), "Raycasting", sf::Style::Resize);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    Arrow arrow(window.getSize().x, window.getSize().y);
    arrow.setOrigin(arrow.x, arrow.y);
    arrow.setPosition(arrow.x, arrow.y);
    arrow.setRotation(arrow.a);

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

        arrow.update();
        arrow.setPosition(arrow.x, arrow.y);
        arrow.setRotation(arrow.a);

        window.clear(sf::Color::Blue);
        window.draw(arrow);
        window.display();
    }

    return 0;
}