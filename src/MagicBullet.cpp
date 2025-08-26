#include "../include/MagicBullet.h"

MagicBullet::MagicBullet(float x, float y, sf::Vector2f direction)
    : Weapon(x, y, direction, WeaponType::MAGIC_BULLET) {

    // MagicBullet-specific properties
    shape.setRadius(8.f); // Larger than normal bullet (4.f)
    shape.setFillColor(sf::Color::Magenta); // Magic color
    shape.setOrigin(8.f, 8.f); // Center origin

    speed = 350.f; // Slightly slower than normal bullets (400.f)
    damage = 15; // Lower damage than GunHero bullets (25)

    // Recalculate velocity with new speed
    calculateVelocity(direction);
}

void MagicBullet::update() {
    moveWeapon(); // Use parent's movement function
}

void MagicBullet::draw(sf::RenderWindow &window) {
    window.draw(shape);
}
