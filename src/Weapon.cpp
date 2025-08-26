#include "../include/Weapon.h"
#include <cmath>

Weapon::Weapon(float x, float y, sf::Vector2f direction, WeaponType type) : BaseObject() {
    weaponType = type;
    shape.setPosition(x, y);

    // Default values - will be overridden by derived classes
    speed = 300.f;
    damage = 10;

    calculateVelocity(direction);
}

sf::Vector2f Weapon::getPosition() const {
    return shape.getPosition();
}

sf::FloatRect Weapon::getBounds() const {
    return shape.getGlobalBounds();
}

void Weapon::setPosition(sf::Vector2f position) {
    shape.setPosition(position);
}

void Weapon::calculateVelocity(sf::Vector2f direction) {
    // Use BaseObject utility to normalize direction
    sf::Vector2f normalizedDir = BaseObject::normalize(direction);
    setVelocity(normalizedDir * speed); // Use BaseObject's setVelocity
}

void Weapon::moveWeapon() {
    // Move weapon based on velocity (assuming 60 FPS)
    sf::Vector2f vel = getVelocity(); // Use BaseObject's getVelocity
    shape.move(vel * (1.f/60.f));
}
