#ifndef ARROW_H
#define ARROW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "ray.hpp"
using namespace std;

class Arrow : public sf::Drawable, public sf::Transformable {
    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        
    public:
        float x;
        float y;
        float a;
        float movementSpeed;
        float rotationalSpeed;
        float width;
        float height;
        vector<Ray> rays;

        sf::VertexArray vertices;
        sf::RectangleShape collisionBox;
        
        Arrow(unsigned int windowWidth, unsigned int windowHeight);
};

#endif