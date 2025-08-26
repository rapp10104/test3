#include "../include/Sword.h"
#include <cmath>
#include <iostream>

Sword::Sword(float x, float y, sf::Vector2f direction)
    : Weapon(x, y, direction, WeaponType::SWORD) {
    
    // Sword-specific properties
    shape.setRadius(8.f); // Small circle for sword handle
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(8.f, 8.f);
    
    // Sword blade shape
    swordShape.setSize(sf::Vector2f(40.f, 8.f)); // Length x Width
    swordShape.setFillColor(sf::Color::White);
    swordShape.setOrigin(0.f, 4.f); // Origin at handle end
    
    speed = 0.f; // Sword doesn't move like projectiles
    damage = 35; // Higher damage than bullets
    
    isSwinging = false;
    swingDuration = 200.f; // 200ms swing duration
    swingAngle = 0.f;
    baseAngle = 0.f;
    swingDirection = direction;

    // Special skill setup
    isUsingSpecialSkill = false;
    specialSlashShape.setSize(sf::Vector2f(120.f, 20.f)); // Much larger than normal sword
    specialSlashShape.setFillColor(sf::Color(255, 255, 0, 150)); // Semi-transparent yellow
    specialSlashShape.setOrigin(0.f, 10.f);
}

void Sword::update() {
    if (isSwinging) {
        float elapsed = swingTimer.getElapsedTime().asMilliseconds();
        
        if (elapsed >= swingDuration) {
            // Swing finished
            isSwinging = false;
            swingAngle = 0.f;
        } else {
            // Calculate swing progress (0.0 to 1.0)
            float progress = elapsed / swingDuration;
            
            // Swing in an arc from -45 to +45 degrees
            swingAngle = (progress - 0.5f) * 90.f; // -45 to +45 degrees
        }
    }
    
    // Update sword blade position and rotation
    sf::Vector2f pos = shape.getPosition();
    swordShape.setPosition(pos);
    
    // Calculate sword rotation based on swing direction and swing angle
    float directionAngle = std::atan2(swingDirection.y, swingDirection.x) * 180.f / 3.14159f;
    swordShape.setRotation(directionAngle + swingAngle);
}

void Sword::draw(sf::RenderWindow &window) {
    if (isUsingSpecialSkill) {
        // Draw special skill slash (large and glowing)
        window.draw(specialSlashShape);
        // Draw handle
        window.draw(shape);
    } else if (isSwinging) {
        // Draw normal sword blade when swinging
        window.draw(swordShape);
        // Draw handle (small circle)
        window.draw(shape);
    }
}

void Sword::startSwing(sf::Vector2f direction) {
    if (!isSwinging) {
        isSwinging = true;
        swingDirection = direction;
        swingTimer.restart();
        swingAngle = 0.f;
        
        // Calculate base angle for swing direction
        baseAngle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    }
}

void Sword::stopSwing() {
    isSwinging = false;
    swingAngle = 0.f;
}

sf::FloatRect Sword::getSwingArea() const {
    if (!isSwinging) {
        return sf::FloatRect(0, 0, 0, 0); // No area when not swinging
    }
    
    // Return area covered by sword swing
    sf::Vector2f pos = shape.getPosition();
    float radius = 40.f; // Sword length
    
    return sf::FloatRect(
        pos.x - radius,
        pos.y - radius,
        radius * 2,
        radius * 2
    );
}

sf::FloatRect Sword::getBounds() const {
    if (isSwinging) {
        return getSwingArea();
    } else {
        return shape.getGlobalBounds(); // Just the handle when not swinging
    }
}

void Sword::setPosition(sf::Vector2f position) {
    shape.setPosition(position);
    swordShape.setPosition(position);
    specialSlashShape.setPosition(position);
}

void Sword::startSpecialSkill(sf::Vector2f direction) {
    isUsingSpecialSkill = true;
    swingDirection = direction;

    // Position and rotate the special slash
    sf::Vector2f pos = shape.getPosition();
    specialSlashShape.setPosition(pos);

    float directionAngle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    specialSlashShape.setRotation(directionAngle);
}

void Sword::stopSpecialSkill() {
    isUsingSpecialSkill = false;
}

sf::FloatRect Sword::getSpecialSkillArea() const {
    if (!isUsingSpecialSkill) {
        return sf::FloatRect(0, 0, 0, 0);
    }

    // Return much larger area for special skill
    sf::Vector2f pos = shape.getPosition();
    float radius = 120.f; // Much larger than normal sword (40.f)

    return sf::FloatRect(
        pos.x - radius,
        pos.y - radius,
        radius * 2,
        radius * 2
    );
}
