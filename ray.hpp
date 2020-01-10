#ifndef RAY_H
#define RAY_H

#include <SFML/Graphics.hpp>

class Ray : public sf::Drawable {
    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            
    public:
        float x;
        float y;
        float a;
        sf::Vector2f direction;
        sf::Vector2f endpoint;
        sf::VertexArray vertices;
        
        Ray(float x, float y, float a);

        sf::Vector2f cast(float x1, float y1, float x2, float y2);
};

#endif