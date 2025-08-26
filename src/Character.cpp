#include "../include/Character.h"
#include "../include/AnimationConfig.h"
#include "../include/Map.h"
#include <cmath>
#include <iostream>

Character::Character() : BaseObject() {
    level = 1;
    exp = 0;
    expToNextLevel = static_cast<int>(10 * std::pow(2, 1.5f)); // Level 2 requirement: ~28 exp

    maxHp = 100; hp = maxHp;
    maxMana = 50; mana = maxMana;
    armor = 0;
    speed = 150.f; // pixels per second
    attackDamage = 10;

    // BaseObject already initializes isAlive and velocity

    // Initialize combat system with default values
    attackCooldownMs = 500;  // Default 0.5 second
    skillCooldownMs = 3000;  // Default 3 seconds
    skillUnlockLevel = 10;   // Default unlock at level 10
    aimDirection = sf::Vector2f(0.f, -1.f); // Default aim up

    shape.setSize(sf::Vector2f(24.f, 24.f)); // Keep for backward compatibility
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(12.f, 12.f); // Center origin
}

void Character::draw(sf::RenderWindow &window) {
    if (useAnimation && currentAnimation) {
        // Draw animated sprite using BaseObject render
        BaseObject::render(window);
    }
    // Always draw shape for collision detection and fallback
    // Note: Shape is transparent when animation is active
    if (!useAnimation || !currentAnimation) {
        window.draw(shape);
    }
}





void Character::gainExp(int amount) {
    exp += amount;
    if (exp >= expToNextLevel) {
        exp -= expToNextLevel;
        levelUp();
    }
}

void Character::levelUp() {
    level++;

    // Exponential exp requirement: base * (level^1.5)
    // Level 1->2: 10 exp, Level 2->3: 17 exp, Level 3->4: 27 exp, etc.
    expToNextLevel = static_cast<int>(10 * std::pow(level, 1.5f));

    maxHp += 10;
    hp = maxHp; // Full heal on level up
    speed += 5.f; // Increase speed
    attackDamage += 2;
}

void Character::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - armor);
    hp -= actualDamage;

    if (hp <= 0) {
        hp = 0;
        isAlive = false;
    }
}

void Character::heal(int amount) {
    hp = std::min(maxHp, hp + amount);
}

void Character::move(sf::Vector2f direction, const Map* map) {
    if (!isAlive) return;

    // Normalize direction
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length;
    }

    // Calculate new position
    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2f newPosition = currentPos + direction * speed * (1.f/60.f); // Assuming 60 FPS

    // Check collision
    if (!checkCollision(newPosition, map)) {
        setPosition(newPosition.x, newPosition.y);
    }
}

bool Character::checkCollision(sf::Vector2f newPosition, const Map* map) const {
    if (!map) return false;

    // Kiểm tra 4 góc của character
    float halfSize = 12.f; // Half of character size

    sf::Vector2f corners[4] = {
        sf::Vector2f(newPosition.x - halfSize, newPosition.y - halfSize), // Top-left
        sf::Vector2f(newPosition.x + halfSize, newPosition.y - halfSize), // Top-right
        sf::Vector2f(newPosition.x - halfSize, newPosition.y + halfSize), // Bottom-left
        sf::Vector2f(newPosition.x + halfSize, newPosition.y + halfSize)  // Bottom-right
    };

    for (int i = 0; i < 4; i++) {
        if (!map->isPassable(corners[i])) {
            return true; // Collision detected
        }
    }

    return false; // No collision
}

bool Character::canAttack() const {
    return attackCooldown.getElapsedTime().asMilliseconds() > attackCooldownMs;
}

bool Character::canUseSkill() const {
    return level >= skillUnlockLevel && skillCooldown.getElapsedTime().asMilliseconds() > skillCooldownMs;
}

void Character::handleMovementInput(const Map* map) {
    if (!getIsAlive()) return;

    // Use Input system for movement
    sf::Vector2f direction = Input::getMovementDirection();

    // Move with collision detection
    move(direction, map);
}

void Character::drawAimIndicator(sf::RenderWindow& window, sf::Color color, float length, float thickness) {
    // Draw aim indicator line
    sf::RectangleShape aimLine(sf::Vector2f(length, thickness));
    aimLine.setOrigin(0.f, thickness / 2.f);
    aimLine.setPosition(shape.getPosition());

    // Calculate rotation angle
    float angle = std::atan2(aimDirection.y, aimDirection.x) * 180.f / 3.14159f;
    aimLine.setRotation(angle);
    aimLine.setFillColor(color);

    window.draw(aimLine);
}

void Character::performAttack() {
    if (!canAttack()) return;

    attackCooldown.restart();
    executeAttack(); // Call derived class implementation
}

void Character::performSkill() {
    if (!canUseSkill()) return;

    skillCooldown.restart();
    executeSkill(); // Call derived class implementation
}

// Template method moved to header file for proper instantiation

void Character::setupPlayerAnimation(const std::string& spritesheetPath, int frameWidth, int frameHeight, int frameCount) {
    if (loadTexture(spritesheetPath)) {
        // Create animation with the loaded texture using config values
        Animation* anim = new Animation(&texture, frameWidth, frameHeight, frameCount, PLAYER_FRAME_DURATION, ANIMATION_ENABLE_LOOP);
        setAnimation(anim);
        enableAnimation(true);
        
        // Set sprite texture and origin
        sprite.setTexture(texture);
        sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
        
        // Apply scale from config to make texture fit game window
        setAnimationScale(PLAYER_ANIMATION_SCALE_X, PLAYER_ANIMATION_SCALE_Y);
        
        // Set texture quality settings
        if (currentAnimation) {
            currentAnimation->setSmooth(ANIMATION_SMOOTH_TEXTURE);
            currentAnimation->setRepeated(ANIMATION_REPEATED_TEXTURE);
        }
        
        // Update shape position to match sprite
        shape.setPosition(sprite.getPosition());
        
        #if ANIMATION_DEBUG_MODE
        std::cout << "Player animation setup: " << frameWidth << "x" << frameHeight 
                  << " frames: " << frameCount << " scale: " << PLAYER_ANIMATION_SCALE_X 
                  << "x" << PLAYER_ANIMATION_SCALE_Y << std::endl;
        std::cout << "Player texture loaded: " << spritesheetPath << std::endl;
        std::cout << "Player sprite texture: " << (sprite.getTexture() ? "OK" : "NULL") << std::endl;
        std::cout << "Player animation enabled: " << (useAnimation ? "YES" : "NO") << std::endl;
        #endif
    } else {
        #if ANIMATION_DEBUG_MODE
        std::cout << "ERROR: Failed to load player texture: " << spritesheetPath << std::endl;
        #endif
    }
}
