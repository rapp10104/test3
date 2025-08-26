#include "../include/MagicHero.h"
#include "../include/AnimationConfig.h"
#include "../include/Map.h"
#include <iostream>
#include <cmath>

MagicHero::MagicHero() {
    shape.setFillColor(sf::Color::Magenta); // Magic color

    // Set MagicHero-specific combat parameters
    attackCooldownMs = 400;  // 0.4 second cooldown (between sword and gun)
    skillCooldownMs = 4000;  // 4 second cooldown
    skillUnlockLevel = 5;    // Unlock earlier at level 5
    
    // Setup MagicHero animation from spritesheet using config values
    setupPlayerAnimation("../assets/Gun_player_spritesheet.png", PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, PLAYER_FRAME_COUNT);
}

void MagicHero::update(const Map* map) {
    // Use Character's common movement handling
    handleMovementInput(map);

    // Update magic bullets
    updateBullets(map);
}

void MagicHero::updateBullets(const Map* map) {
    // Use Character's template method for bullet management
    updateBulletContainer(magicBullets, map);
}

void MagicHero::executeAttack() {
    // Implementation without cooldown check (handled by template method)

    // Create magic bullet in aim direction
    sf::Vector2f bulletPos = shape.getPosition() + aimDirection * 25.f;
    magicBullets.push_back(MagicBullet(bulletPos.x, bulletPos.y, aimDirection));
}

void MagicHero::executeSkill() {
    // Implementation without cooldown check (handled by template method)

    // Shoot 3 large magic bullets in spread pattern
    for (int i = -1; i <= 1; i++) {
        float angle = i * 0.5f; // 0.5 radian spread (wider than GunHero)
        sf::Vector2f skillDirection(
            aimDirection.x * cos(angle) - aimDirection.y * sin(angle),
            aimDirection.x * sin(angle) + aimDirection.y * cos(angle)
        );

        sf::Vector2f bulletPos = shape.getPosition() + skillDirection * 25.f;
        magicBullets.push_back(MagicBullet(bulletPos.x, bulletPos.y, skillDirection));
    }
}

void MagicHero::draw(sf::RenderWindow &window) {
    // Draw character (use parent's draw method for WebP support)
    Character::draw(window);

    // Draw magic bullets
    for (auto &b : magicBullets) {
        b.draw(window);
    }

    // Draw aim indicator using Character's template method
    drawAimIndicator(window, sf::Color::Cyan, 35.f, 3.f);
}

// Removed - now uses Character::canAttack() and Character::canUseSkill()
