#include "../include/Camera.h"
#include <iostream>

Camera::Camera(float width, float height) {
    viewSize = sf::Vector2f(width, height);
    view.setSize(viewSize);
    center = sf::Vector2f(width / 2.0f, height / 2.0f);
    view.setCenter(center);
}

void Camera::follow(const sf::Vector2f& playerPosition) {
    center = playerPosition;
    view.setCenter(center);
}

void Camera::apply(sf::RenderWindow& window) {
    window.setView(view);
}

void Camera::setCenter(const sf::Vector2f& newCenter) {
    center = newCenter;
    view.setCenter(center);
}

void Camera::setViewSize(float width, float height) {
    viewSize = sf::Vector2f(width, height);
    view.setSize(viewSize);
}

sf::Vector2f Camera::worldToScreen(const sf::Vector2f& worldPos) const {
    // Convert world coordinates to screen coordinates
    sf::Vector2f offset = worldPos - center;
    return sf::Vector2f(viewSize.x / 2.0f + offset.x, viewSize.y / 2.0f + offset.y);
}

sf::Vector2f Camera::screenToWorld(const sf::Vector2f& screenPos) const {
    // Convert screen coordinates to world coordinates
    sf::Vector2f offset = screenPos - sf::Vector2f(viewSize.x / 2.0f, viewSize.y / 2.0f);
    return center + offset;
}
