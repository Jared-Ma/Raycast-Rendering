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
        // void setOutlineColor(const Color);

    public:
        float x;    // x position
        float y;    // y position
        float dx;   // change in x (speed)
        float dy;   // change in y (speed)
        float a;    // heading angle
        float da;   // change in rotation
        int moving; // 1: forward, 0: stationary, -1: backward
        
        float acceleration;
        float rotationalSpeed;
        float maxSpeed;
        
        Arrow(unsigned int width, unsigned int height) {
            x = width / 2;
            y = height / 2;
            dx = 0;
            dy = 0;
            a = 90;
            da = 0;
            moving = 0;

            acceleration = 0.1;
            rotationalSpeed = 5;
            maxSpeed = 0.0000001;
            
            vertices.setPrimitiveType(sf::Triangles);
            vertices.resize(3);
            vertices[0].position = sf::Vector2f(x + 25, y);
            vertices[1].position = sf::Vector2f(x - 25, y + 20);
            vertices[2].position = sf::Vector2f(x - 25, y - 20);
        }

        void update() {
            x += dx;
            y += dy;
            a += da;

            // dx += moving * 1 * cos(degToRad(a));
            // dy += moving * 1 * sin(degToRad(a));

            dx += min(moving * acceleration * cos(degToRad(a)), maxSpeed * cos(degToRad(a)));
            cout << "dx: " << dx << endl;
            dy += min(moving * acceleration * sin(degToRad(a)), maxSpeed * sin(degToRad(a)));
            cout << "dy: " << dx << endl;

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
                        cout << "left press" << endl;
                        arrow.da = arrow.rotationalSpeed;
                        break;
                    case sf::Keyboard::Right:
                        cout << "right press" << endl;
                        arrow.da = -arrow.rotationalSpeed;
                        break;
                    case sf::Keyboard::Up:
                        arrow.moving = 1;
                        cout << "up press" << endl;
                        break;
                    case sf::Keyboard::Down:
                        arrow.moving = -1;
                        cout << "down press" << endl;
                        break;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                switch(event.key.code) {
                    case sf::Keyboard::Left:
                        cout << "left release" << endl;
                        // arrow.toggleRotate();
                        // cout << arrow.rotating << endl;
                        arrow.da = 0;
                        break;
                    case sf::Keyboard::Right:
                        cout << "right release" << endl;
                        // arrow.toggleRotate();
                        // cout << arrow.rotating << endl;
                        arrow.da = 0;
                        break;
                    case sf::Keyboard::Up:
                        arrow.moving = 0;
                        arrow.dx = 0;
                        arrow.dy = 0;
                        cout << "up release" << endl;
                        break;
                    case sf::Keyboard::Down:
                        arrow.moving = 0;
                        arrow.dx = 0;
                        arrow.dy = 0;
                        cout << "down release" << endl;
                        break;
                }
            }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            //     cout << "up" << endl;
            //     arrow.move(3 * cos(degToRad(arrow.getRotation())), 3 * sin(degToRad(arrow.getRotation())));
            // }
            // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            //     cout << "down" << endl;
            //     arrow.move(-3 * cos(degToRad(arrow.getRotation())), -3 * sin(degToRad(arrow.getRotation())));
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            //     cout << "left" << endl;
            //     arrow.rotate(-5.f);
            // }
            // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            //     cout << "right" << endl;
            //     arrow.rotate(5.f);
            // }
            // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            //     window.close();
            // }
        }
        arrow.update();
        arrow.setPosition(arrow.x, arrow.y);
        // arrow.move();
        arrow.setRotation(arrow.a);
        // arrow.rotate();

        window.clear(sf::Color::Blue);
        window.draw(arrow);
        window.display();
    }

    return 0;
}