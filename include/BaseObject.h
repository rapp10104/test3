#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Animation.h"
#include "AnimationConfig.h"

enum class ObjectType {
    CHARACTER,
    ENEMY,
    WEAPON,
    PROJECTILE
};

// Enhanced base class for all game objects
class BaseObject {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    bool isAlive;
    
    // Animation support
    Animation* currentAnimation;
    bool useAnimation;

public:
    BaseObject() : isAlive(true), currentAnimation(nullptr), useAnimation(false) {}
    BaseObject(const BaseObject& other) : isAlive(other.isAlive), currentAnimation(nullptr), useAnimation(false) {
        // QUAN TRỌNG: Không copy animation - sẽ được setup lại
        sprite = other.sprite;
        texture = other.texture;
        velocity = other.velocity;
    }
    
    BaseObject& operator=(const BaseObject& other) {
        if (this != &other) {
            // Cleanup animation hiện tại trước
            if (currentAnimation) {
                try {
                    currentAnimation->setTexture(nullptr);
                    delete currentAnimation;
                } catch (...) {
                    // Ignore exceptions during cleanup
                }
                currentAnimation = nullptr;
            }
            
            // Copy các thành phần khác
            isAlive = other.isAlive;
            sprite = other.sprite;
            texture = other.texture;
            velocity = other.velocity;
            useAnimation = false; // Reset animation state
        }
        return *this;
    }
    virtual ~BaseObject() {
        if (currentAnimation) {
            try {
                // Reset texture reference trước khi delete để tránh crash
                currentAnimation->setTexture(nullptr);
                delete currentAnimation;
                currentAnimation = nullptr;
            } catch (...) {
                // Ignore any exceptions during cleanup
                currentAnimation = nullptr;
            }
        }
    }

    // Texture and sprite management
    bool loadTexture(const std::string& file) {
        return texture.loadFromFile(file);
    }

    void setSprite(const sf::Texture& tex) {
        sprite.setTexture(tex);
    }

    // Position and movement (virtual for derived classes to override)
    virtual void setPosition(float x, float y) {
        // This base implementation sets sprite position
        // Derived classes should call this first, then set their shape position
        sprite.setPosition(x, y);
    }

    virtual sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    virtual sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    // Velocity and state
    void setVelocity(sf::Vector2f vel) { velocity = vel; }
    sf::Vector2f getVelocity() const { return velocity; }
    void setAlive(bool alive) { isAlive = alive; }
    bool getAlive() const { return isAlive; }
    virtual bool getIsAlive() const { return isAlive; } // Virtual for consistency

    // Convenience methods
    void move(sf::Vector2f offset) { sprite.move(offset); }
    void setOrigin(float x, float y) { sprite.setOrigin(x, y); }
    void setScale(float x, float y) { sprite.setScale(x, y); }
    void setRotation(float angle) { sprite.setRotation(angle); }

    // Virtual methods for derived classes
    virtual void update(float dt) {
        sprite.move(velocity * dt);
        
        // Update animation if active - với kiểm tra an toàn
        if (useAnimation && currentAnimation) {
            try {
                currentAnimation->update(dt);
                sf::IntRect currentFrame = currentAnimation->getCurrentFrame();
                if (currentFrame.width > 0 && currentFrame.height > 0) {
                    sprite.setTextureRect(currentFrame);
                }
            } catch (...) {
                // Nếu có lỗi, disable animation
                useAnimation = false;
                #if ANIMATION_DEBUG_MODE
                std::cout << "WARNING: Animation error, disabling animation" << std::endl;
                #endif
            }
        }
    }

    virtual void render(sf::RenderWindow& window) {
        if (isAlive && useAnimation && currentAnimation && sprite.getTexture()) {
            window.draw(sprite);
        }
    }

    // Static utility functions for game objects
    static float distanceBetween(sf::Vector2f pos1, sf::Vector2f pos2);
    static bool intersects(sf::FloatRect bounds1, sf::FloatRect bounds2);
    static sf::Vector2f normalize(sf::Vector2f vector);
    static float vectorLength(sf::Vector2f vector);

    // Helper functions for common operations
    static sf::Vector2f calculateDirection(sf::Vector2f from, sf::Vector2f to);
    static bool isInRange(sf::Vector2f pos1, sf::Vector2f pos2, float range);
    
    // Animation methods
    void setAnimation(Animation* animation);
    void enableAnimation(bool enable);
    Animation* getCurrentAnimation() const;
    bool isAnimationEnabled() const;
    
    // Animation scale methods
    void setAnimationScale(float scaleX, float scaleY);
    void setAnimationScale(float scale);
    sf::Vector2f getAnimationScale() const;
};

#endif
