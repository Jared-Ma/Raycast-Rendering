#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall : public sf::Drawable {
    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            
    public:
        sf::Vector2f p1;
        sf::Vector2f p2;
        sf::VertexArray vertices;
        
        Wall(float x1, float y1, float x2, float y2);
};

#endif