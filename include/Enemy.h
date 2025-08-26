#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "BaseObject.h"

class Map; // Forward declaration

class Enemy : public BaseObject {
private:
    sf::RectangleShape shape; // Keep for backward compatibility
    float speed;
    int hp, maxHp;
    int damage;
    sf::Clock attackCooldown;

    // AI behavior
    sf::Vector2f targetPosition;
    float detectionRange;
    float attackRange;

public:
    Enemy(float x, float y);
    ~Enemy(); // Destructor để cleanup animation
    Enemy(const Enemy& other); // Copy constructor
    Enemy& operator=(const Enemy& other); // Assignment operator
    void update(sf::Vector2f playerPos, const Map* map);
    void draw(sf::RenderWindow &window);
    void takeDamage(int damage);

    // Getters (override BaseObject methods for backward compatibility)
    sf::Vector2f getPosition() const override { return shape.getPosition(); }
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    bool getIsAlive() const override { return BaseObject::getIsAlive(); }
    
    // Override setPosition to sync sprite and shape
    void setPosition(float x, float y) override;
    int getDamage() const { return damage; }
    bool canAttack() const;

    // Combat
    bool isInAttackRange(sf::Vector2f playerPos) const;
    void attackPlayer();
    
    // Animation support
    void setupEnemyAnimation(const std::string& spritesheetPath, int frameWidth, int frameHeight, int frameCount);
};

#endif
