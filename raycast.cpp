#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

float degToRad(float deg) {
    return deg * M_PI / 180;
}

float radToDeg(float rad) {
    return rad * 180 / M_PI;
}

bool lineLineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    float a = ((y1-y3)*(x4-x3) - (x1-x3)*(y4-y3)) / ((x2-x1)*(y4-y3) - (y2-y1)*(x4-x3));
    float b = ((y1-y3)*(x2-x1) - (x1-x3)*(y2-y1)) / ((x2-x1)*(y4-y3) - (y2-y1)*(x4-x3));

    cout << a << endl;
    cout << b << endl;

    if (a >= 0 && a <= 1 && b >= 0 && b <= 1) {
        return true;
    }
    else {
        return false;
    }
}

// bool lineRectCollision(sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect rect) {
    // bool left = lineLineCollision(p1.x, p1.y, p2.x, p2.y, rect.top, rect.left, rect.top + rect.height, rect.left);
    // bool right = lineLineCollision(p1.x, p1.y, p2.x, p2.y, rect.top, rect.left + rect.width, rect.top + rect.height, rect.left + rect.width);
    // bool top = lineLineCollision(p1.x, p1.y, p2.x, p2.y, rect.top, rect.left, rect.top, rect.left + rect.height);
    // bool bottom = lineLineCollision(p1.x, p1.y, p2.x, p2.y, rect.top + rect.height, rect.left, rect.top + rect.height, rect.left + rect.width);
bool lineRectCollision(float x1, float y1, float x2, float y2, sf::FloatRect rect) {
    bool left = lineLineCollision(x1, y1, x2, y2, rect.top, rect.left, rect.top + rect.height, rect.left);
    bool right = lineLineCollision(x1, y1, x2, y2, rect.top, rect.left + rect.width, rect.top + rect.height, rect.left + rect.width);
    bool top = lineLineCollision(x1, y1, x2, y2, rect.top, rect.left, rect.top, rect.left + rect.height);
    bool bottom = lineLineCollision(x1, y1, x2, y2, rect.top + rect.height, rect.left, rect.top + rect.height, rect.left + rect.width);

    // cout << x1 << endl;
    // cout << y1 << endl;
    // cout << x2 << endl;
    // cout << y2 << endl;
    // cout << "left: " << left << endl;
    // cout << "right: " << right << endl;
    // cout << "top: " << top << endl;
    // cout << "bottom: " << bottom << endl;

    if (left || right || top || bottom) {
        return true;
    }
    else {
        return false;
    }
}

class Arrow : public sf::Drawable, public sf::Transformable {
    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
            states.transform *= getTransform();
            target.draw(vertices, states);
        }
        
    public:
        float x;
        float y;
        float a;
        float movementSpeed;
        float rotationalSpeed;

        sf::VertexArray vertices;
        sf::FloatRect boundingBox;
        
        Arrow(unsigned int width, unsigned int height) {
            x = width / 2;
            y = height / 2;
            a = 0;
            movementSpeed = 0;
            rotationalSpeed = 0;
            
            vertices.setPrimitiveType(sf::Triangles);
            vertices.resize(3);
            vertices[0].position = sf::Vector2f(x + 10, y);
            vertices[1].position = sf::Vector2f(x - 10, y + 7);
            vertices[2].position = sf::Vector2f(x - 10, y - 7);
            boundingBox = vertices.getBounds();
        }

        void update() {

            float dx = movementSpeed * cos(degToRad(a));
            float dy = movementSpeed * sin(degToRad(a));

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
            boundingBox = vertices.getBounds();
        }
};

// class Ray {

// }

class Wall : public sf::Drawable {
    private:
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
                target.draw(vertices, states);
            }
            
    public:
        float x1;
        float y1;
        float x2;
        float y2;
        sf::VertexArray vertices;
        sf::FloatRect boundingBox;
        
        Wall(float x1, float y1, float x2, float y2) {
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;

            vertices.setPrimitiveType(sf::Lines);
            vertices.resize(2);
            vertices[0].position = sf::Vector2f(x1, y1);
            vertices[1].position = sf::Vector2f(x2, y2);
            boundingBox = vertices.getBounds();
        }
};



int main(){
    sf::RenderWindow window(sf::VideoMode(500, 500), "Raycasting", sf::Style::Resize);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    Arrow arrow(window.getSize().x, window.getSize().y);

    vector<Wall> walls;
    walls.push_back(Wall(100, 100, 400, 100));
    // walls.push_back(Wall(400, 100, 400, 400));
    // walls.push_back(Wall(400, 400, 100, 400));
    // walls.push_back(Wall(100, 400, 100, 100));

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

        // if (lineRectCollision(walls[0].vertices[0].position, walls[0].vertices[1].position, arrow.boundingBox)) {
        //     cout << "collision" << endl;
        // }
        if (lineRectCollision(walls[0].x1, walls[0].y1, walls[0].x2, walls[0].y2, arrow.boundingBox)) {
            cout << "collision" << endl;
        }
        else {
            cout << "no collision" << endl;
        }

        

        arrow.update();
        sf::RectangleShape rectangle;
        rectangle.setOutlineThickness(1);
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setSize(sf::Vector2f(arrow.boundingBox.width, arrow.boundingBox.height));
        rectangle.setPosition(arrow.boundingBox.left, arrow.boundingBox.top);

        window.clear(sf::Color::Blue);
        for (auto wall = walls.begin(); wall != walls.end(); wall++) {
            window.draw(*wall);
        }
        window.draw(rectangle);
        window.draw(arrow.vertices);
        
        window.display();
    }

    return 0;
}