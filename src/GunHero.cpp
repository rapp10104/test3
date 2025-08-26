#include "../include/GunHero.h"
#include "../include/AnimationConfig.h"
#include "../include/Map.h"
#include <iostream>
#include <cmath>

GunHero::GunHero() {
    shape.setFillColor(sf::Color::Green); // màu xanh lá

    // Set GunHero-specific combat parameters
    attackCooldownMs = 300;  // 0.3 second cooldown (faster than sword)
    skillCooldownMs = 5000;  // 5 second cooldown
    skillUnlockLevel = 10;   // Unlock at level 10
    
    // Setup GunHero animation from spritesheet using config values
    setupPlayerAnimation("../assets/Gun_player_spritesheet.png", PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, PLAYER_FRAME_COUNT);
}

void GunHero::update(const Map* map) {
    // Use Character's common movement handling
    handleMovementInput(map);

    // Update bullets
    updateBullets(map);
}

void GunHero::updateBullets(const Map* map) {
    // Use Character's template method for bullet management
    updateBulletContainer(bullets, map);
}

void GunHero::executeAttack() {
    // Implementation without cooldown check (handled by template method)

    // Create bullet in aim direction
    sf::Vector2f bulletPos = shape.getPosition() + aimDirection * 20.f;
    bullets.push_back(Bullet(bulletPos.x, bulletPos.y, aimDirection));
}

void GunHero::executeSkill() {
    // Implementation without cooldown check (handled by template method)

    // Shoot 5 bullets in spread pattern
    for (int i = -2; i <= 2; i++) {
        float angle = i * 0.3f; // 0.3 radian spread
        sf::Vector2f skillDirection(
            aimDirection.x * cos(angle) - aimDirection.y * sin(angle),
            aimDirection.x * sin(angle) + aimDirection.y * cos(angle)
        );

        sf::Vector2f bulletPos = shape.getPosition() + skillDirection * 20.f;
        bullets.push_back(Bullet(bulletPos.x, bulletPos.y, skillDirection));
    }
}

void GunHero::draw(sf::RenderWindow &window) {
    // Draw character (use parent's draw method for WebP support)
    Character::draw(window);

    // Draw bullets
    for (auto &b : bullets) {
        b.draw(window);
    }

    // Draw aim indicator using Character's template method
    drawAimIndicator(window, sf::Color::Yellow, 30.f, 2.f);
}

// Removed - now uses Character::canAttack() and Character::canUseSkill()
