#ifndef RAY_H
#define RAY_H

#include <SFML/Graphics.hpp>

class Ray : public sf::Drawable, public sf::Transformable {
    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            
    public:
        float x;
        float y;
        sf::Vector2f direction;
        
        Ray(float x, float y, float a);

        bool cast(float x1, float y1, float x2, float y2);
};

#endif