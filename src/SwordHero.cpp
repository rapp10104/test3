#include "../include/SwordHero.h"
#include "../include/AnimationConfig.h"
#include "../include/Map.h"
#include <iostream>
#include <cmath>

SwordHero::SwordHero() {
    shape.setFillColor(sf::Color::Blue); // màu xanh để phân biệt
    isUsingSpecialSkill = false;
    hasUsedSkillBefore = false;

    // Set SwordHero-specific combat parameters
    attackCooldownMs = 500;  // 0.5 second cooldown
    skillCooldownMs = 3000;  // 3 second cooldown
    skillUnlockLevel = 10;   // Unlock at level 10

    // Create sword weapon
    sf::Vector2f pos = shape.getPosition();
    sword = std::make_unique<Sword>(pos.x, pos.y, aimDirection);
    
    // Setup SwordHero animation from spritesheet using config values
    setupPlayerAnimation("../assets/Gun_player_spritesheet.png", PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, PLAYER_FRAME_COUNT);
}

void SwordHero::update(const Map* map) {
    // Use Character's common movement handling
    handleMovementInput(map);

    // Update sword position to follow player
    if (sword) {
        sf::Vector2f pos = shape.getPosition();
        sword->setPosition(pos);
        sword->update();
    }

    // Handle special skill duration
    if (isUsingSpecialSkill && specialSkillTimer.getElapsedTime().asMilliseconds() > 500) {
        isUsingSpecialSkill = false;
        if (sword) {
            sword->stopSpecialSkill();
        }
    }
}

void SwordHero::executeAttack() {
    // Implementation without cooldown check (handled by template method)

    // Start sword swing
    if (sword) {
        sword->startSwing(aimDirection); // Use Character's aimDirection
    }
}

void SwordHero::executeSkill() {
    // Implementation without cooldown check (handled by template method)
    isUsingSpecialSkill = true;
    specialSkillTimer.restart();
    hasUsedSkillBefore = true; // Mark that skill has been used

    // Long range slash - much larger attack area
    if (sword) {
        sword->startSpecialSkill(aimDirection);
    }
}

void SwordHero::draw(sf::RenderWindow &window) {
    // Set color based on attack state
    if (getIsAttacking()) {
        shape.setFillColor(sf::Color::Red); // Flash red when attacking
    } else {
        shape.setFillColor(sf::Color::Blue);
    }

    // Draw character (use parent's draw method for WebP support)
    Character::draw(window);

    // Draw sword
    if (sword) {
        sword->draw(window);
    }

    // Draw aim indicator using Character's template method
    drawAimIndicator(window, sf::Color::Red, 25.f, 2.f);
}

sf::FloatRect SwordHero::getAttackArea() const {
    if (sword && (sword->isSwingActive() || isUsingSpecialSkill)) {
        if (isUsingSpecialSkill) {
            return sword->getSpecialSkillArea();
        } else {
            return sword->getSwingArea();
        }
    }
    return sf::FloatRect(0, 0, 0, 0); // No attack area when not attacking
}

// Removed - now uses Character::canAttack()

bool SwordHero::canUseSkill() const {
    // Allow skill if level >= 10 and (first time use OR cooldown expired)
    return level >= 10 && (!hasUsedSkillBefore || skillCooldown.getElapsedTime().asMilliseconds() > 3000);
}

bool SwordHero::getIsAttacking() const {
    return sword && (sword->isSwingActive() || isUsingSpecialSkill);
}
