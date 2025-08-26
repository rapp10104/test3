#include "../include/Bullet.h"

Bullet::Bullet(float x, float y, sf::Vector2f direction)
    : Weapon(x, y, direction, WeaponType::BULLET) {

    // Bullet-specific properties
    shape.setRadius(4.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(4.f, 4.f); // Center origin

    speed = 400.f; // pixels per second
    damage = 25;

    // Recalculate velocity with new speed
    calculateVelocity(direction);
}

void Bullet::update() {
    moveWeapon(); // Use parent's movement function
}

void Bullet::draw(sf::RenderWindow &window) {
    window.draw(shape);
}
