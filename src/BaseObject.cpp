#include "../include/BaseObject.h"
#include "../include/AnimationConfig.h"
#include <cmath>

float BaseObject::distanceBetween(sf::Vector2f pos1, sf::Vector2f pos2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool BaseObject::intersects(sf::FloatRect bounds1, sf::FloatRect bounds2) {
    return bounds1.intersects(bounds2);
}

sf::Vector2f BaseObject::normalize(sf::Vector2f vector) {
    float length = vectorLength(vector);
    if (length > 0) {
        return vector / length;
    }
    return sf::Vector2f(0.f, 0.f);
}

float BaseObject::vectorLength(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f BaseObject::calculateDirection(sf::Vector2f from, sf::Vector2f to) {
    return normalize(to - from);
}

bool BaseObject::isInRange(sf::Vector2f pos1, sf::Vector2f pos2, float range) {
    return distanceBetween(pos1, pos2) <= range;
}

// Animation methods implementation
void BaseObject::setAnimation(Animation* animation) {
    if (currentAnimation) {
        delete currentAnimation;
    }
    currentAnimation = animation;
}

void BaseObject::enableAnimation(bool enable) {
    useAnimation = enable;
}

Animation* BaseObject::getCurrentAnimation() const {
    return currentAnimation;
}

bool BaseObject::isAnimationEnabled() const {
    return useAnimation;
}

// Animation scale methods implementation
void BaseObject::setAnimationScale(float scaleX, float scaleY) {
    if (useAnimation && currentAnimation) {
        sprite.setScale(scaleX, scaleY);
    }
}

void BaseObject::setAnimationScale(float scale) {
    setAnimationScale(scale, scale);
}

sf::Vector2f BaseObject::getAnimationScale() const {
    if (useAnimation && currentAnimation) {
        return sprite.getScale();
    }
    return sf::Vector2f(1.0f, 1.0f);
}
