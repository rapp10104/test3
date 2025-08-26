#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
private:
    sf::View view;
    sf::Vector2f center;
    sf::Vector2f viewSize;

public:
    Camera(float width, float height);
    
    // Main camera functions
    void follow(const sf::Vector2f& playerPosition);
    void apply(sf::RenderWindow& window);
    
    // Getters
    sf::Vector2f getCenter() const { return center; }
    sf::View getView() const { return view; }
    
    // Setters
    void setCenter(const sf::Vector2f& newCenter);
    void setViewSize(float width, float height);
    
    // Utility functions
    sf::Vector2f worldToScreen(const sf::Vector2f& worldPos) const;
    sf::Vector2f screenToWorld(const sf::Vector2f& screenPos) const;
};

#endif
