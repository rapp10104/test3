#include "../include/Enemy.h"
#include "../include/AnimationConfig.h"
#include "../include/Map.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(float x, float y) : BaseObject() {
    shape.setSize(sf::Vector2f(28.f, 28.f)); // Keep for backward compatibility
    shape.setOrigin(14.f, 14.f); // Center origin
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);

    speed = 80.f; // pixels per second
    maxHp = 50;
    hp = maxHp;
    damage = 15;
    // BaseObject already initializes isAlive and texture

    detectionRange = 500.f;
    attackRange = 20.f; // Increased for better collision
    targetPosition = sf::Vector2f(x, y);
}

Enemy::~Enemy() {
    // BaseObject destructor sẽ handle animation cleanup
}

Enemy::Enemy(const Enemy& other) : BaseObject(other) {
    shape = other.shape;
    speed = other.speed;
    hp = other.hp;
    maxHp = other.maxHp;
    damage = other.damage;
    detectionRange = other.detectionRange;
    attackRange = other.attackRange;
    targetPosition = other.targetPosition;
    attackCooldown = other.attackCooldown;
    
    // QUAN TRỌNG: Không copy animation - sẽ được setup lại
    // BaseObject copy constructor đã set currentAnimation = nullptr
}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this != &other) {
        BaseObject::operator=(other);
        shape = other.shape;
        speed = other.speed;
        hp = other.hp;
        maxHp = other.maxHp;
        damage = other.damage;
        detectionRange = other.detectionRange;
        attackRange = other.attackRange;
        targetPosition = other.targetPosition;
        attackCooldown = other.attackCooldown;
        
        // QUAN TRỌNG: Không copy animation - sẽ được setup lại
        // BaseObject assignment operator đã set currentAnimation = nullptr
    }
    return *this;
}

void Enemy::update(sf::Vector2f playerPos, const Map* map) {
    if (!isAlive) return;

    sf::Vector2f currentPos = shape.getPosition();
    float distanceToPlayer = BaseObject::distanceBetween(currentPos, playerPos);

    // AI Behavior: Chase player if in detection range
    if (distanceToPlayer <= detectionRange) {
        targetPosition = playerPos;

        // Move towards player
        sf::Vector2f direction = BaseObject::calculateDirection(currentPos, targetPosition);

        if (distanceToPlayer > attackRange) {

            // Calculate new position
            sf::Vector2f newPosition = currentPos + direction * speed * (1.f/60.f);

            // Simple collision check with map
            if (map && map->isPassable(newPosition)) {
                setPosition(newPosition.x, newPosition.y);
            }
        }

        // Attack logic will be handled in Game::checkCollisions()
        // Remove duplicate attack logic from here
    }
}



void Enemy::draw(sf::RenderWindow &window) {
    if (!isAlive) return;

    if (useAnimation && currentAnimation && sprite.getTexture()) {
        // Draw animated sprite với texture rect để cắt frame
        sf::Sprite animatedSprite = sprite;
        animatedSprite.setTextureRect(currentAnimation->getCurrentFrame());
        window.draw(animatedSprite);
        
        #if ANIMATION_DEBUG_MODE && ANIMATION_SHOW_FRAME_INFO
        // Debug: hiển thị frame info
        static int lastFrame = -1;
        int currentFrame = currentAnimation->getCurrentFrameIndex();
        if (currentFrame != lastFrame) {
            std::cout << "Enemy frame: " << currentFrame << "/" << currentAnimation->getFrameCount() << std::endl;
            lastFrame = currentFrame;
        }
        #endif
    } else if (sprite.getTexture()) {
        // Draw static texture nếu không có animation
        window.draw(sprite);
    } else {
        // Fallback to shape
        // Change color based on health
        float healthPercent = static_cast<float>(hp) / maxHp;
        if (healthPercent > 0.6f) {
            shape.setFillColor(sf::Color::Red);
        } else if (healthPercent > 0.3f) {
            shape.setFillColor(sf::Color(255, 165, 0)); // Orange
        } else {
            shape.setFillColor(sf::Color(139, 0, 0)); // Dark red
        }
        window.draw(shape);
    }

    // Draw health bar
    float healthPercent = static_cast<float>(hp) / maxHp;
    sf::RectangleShape healthBarBg(sf::Vector2f(24.f, 4.f));
    healthBarBg.setOrigin(12.f, 2.f);
    healthBarBg.setPosition(shape.getPosition().x, shape.getPosition().y - 18.f);
    healthBarBg.setFillColor(sf::Color::Black);
    window.draw(healthBarBg);

    sf::RectangleShape healthBar(sf::Vector2f(22.f * healthPercent, 2.f));
    healthBar.setOrigin(11.f * healthPercent, 1.f);
    healthBar.setPosition(shape.getPosition().x, shape.getPosition().y - 18.f);
    healthBar.setFillColor(sf::Color::Green);
    window.draw(healthBar);
}

void Enemy::takeDamage(int damageAmount) {
    hp -= damageAmount;
    if (hp <= 0) {
        hp = 0;
        setAlive(false); // Use BaseObject method
    }
}

bool Enemy::canAttack() const {
    return attackCooldown.getElapsedTime().asMilliseconds() > 500; // 0.5 second cooldown (faster for testing)
}

bool Enemy::isInAttackRange(sf::Vector2f playerPos) const {
    sf::Vector2f currentPos = shape.getPosition();
    return BaseObject::isInRange(currentPos, playerPos, attackRange);
}

void Enemy::attackPlayer() {
    attackCooldown.restart();
    // Attack animation or sound effect could be added here
}

void Enemy::setPosition(float x, float y) {
    // Gọi BaseObject::setPosition trước để set sprite position
    BaseObject::setPosition(x, y);
    
    // Sau đó set shape position để đồng bộ
    shape.setPosition(x, y);
    
    // Debug: kiểm tra position sync
    #if ANIMATION_DEBUG_MODE
    static sf::Vector2f lastPos(-1, -1);
    if (lastPos.x != x || lastPos.y != y) {
        std::cout << "Enemy position set: (" << x << ", " << y 
                  << ") - Sprite: (" << sprite.getPosition().x << ", " << sprite.getPosition().y
                  << ") - Shape: (" << shape.getPosition().x << ", " << shape.getPosition().y << ")" << std::endl;
        lastPos = sf::Vector2f(x, y);
    }
    #endif
}

void Enemy::setupEnemyAnimation(const std::string& spritesheetPath, int frameWidth, int frameHeight, int frameCount) {
    if (loadTexture(spritesheetPath)) {
        try {
            // Tạo animation với texture đã load
            Animation* anim = new Animation(&texture, frameWidth, frameHeight, frameCount, ENEMY_FRAME_DURATION, ANIMATION_ENABLE_LOOP);
            setAnimation(anim);
            enableAnimation(true);
            
            // Set sprite texture và origin
            sprite.setTexture(texture);
            sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
            
            // QUAN TRỌNG: Scale sprite để phù hợp với game window
            sprite.setScale(ENEMY_ANIMATION_SCALE_X, ENEMY_ANIMATION_SCALE_Y);
            
            // Set texture quality settings
            if (currentAnimation) {
                currentAnimation->setSmooth(ANIMATION_SMOOTH_TEXTURE);
                currentAnimation->setRepeated(ANIMATION_REPEATED_TEXTURE);
            }
            
            // Update shape position để đồng bộ với sprite
            shape.setPosition(sprite.getPosition());
            
                    #if ANIMATION_DEBUG_MODE
        std::cout << "Enemy animation setup: " << frameWidth << "x" << frameHeight 
                  << " frames: " << frameCount << " scale: " << ENEMY_ANIMATION_SCALE_X 
                  << "x" << ENEMY_ANIMATION_SCALE_Y << std::endl;
        std::cout << "Enemy texture loaded: " << spritesheetPath << std::endl;
        std::cout << "Enemy sprite texture: " << (sprite.getTexture() ? "OK" : "NULL") << std::endl;
        std::cout << "Enemy animation: ENABLED" << std::endl;
        std::cout << "Enemy sprite scale: " << sprite.getScale().x << "x" << sprite.getScale().y << std::endl;
        std::cout << "Enemy animation object: " << (currentAnimation ? "OK" : "NULL") << std::endl;
        std::cout << "Enemy frames count: " << (currentAnimation ? currentAnimation->getFrameCount() : 0) << std::endl;
        #endif
        } catch (...) {
            // Nếu có lỗi, disable animation và log
            enableAnimation(false);
            #if ANIMATION_DEBUG_MODE
            std::cout << "ERROR: Failed to setup enemy animation, disabling animation" << std::endl;
            #endif
        }
    } else {
        #if ANIMATION_DEBUG_MODE
        std::cout << "ERROR: Failed to load enemy texture: " << spritesheetPath << std::endl;
        #endif
    }
}
