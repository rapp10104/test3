#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "BaseObject.h"
#include "Map.h" // Include for template method
#include "Input.h" // Include for input handling
#include <string>

class Character : public BaseObject {
protected:
    sf::RectangleShape shape; // Keep for backward compatibility, could migrate to BaseObject::sprite later

    int level;
    int exp;
    int expToNextLevel;

    int hp, maxHp;
    int mana, maxMana;
    int armor;
    float speed;
    int attackDamage;

    // Combat system
    sf::Clock attackCooldown;
    sf::Clock skillCooldown;
    sf::Vector2f aimDirection; // Unified aim direction for all heroes

    // Combat parameters (can be overridden by derived classes)
    int attackCooldownMs;
    int skillCooldownMs;
    int skillUnlockLevel;

    // Character-specific properties
    std::string name;

public:
    Character();
    virtual ~Character() {}

    virtual void update(const Map* map) = 0;
    virtual void draw(sf::RenderWindow &window);
    virtual void attack() = 0; // tấn công thường
    virtual void skill() = 0;  // kỹ năng đặc biệt

    void gainExp(int amount);
    void levelUp();
    void takeDamage(int damage);
    void heal(int amount);

    // Common combat methods (can be overridden if needed)
    virtual bool canAttack() const;
    virtual bool canUseSkill() const;

    // Template methods for common functionality
    virtual void handleMovementInput(const Map* map);
    virtual void drawAimIndicator(sf::RenderWindow& window, sf::Color color, float length = 30.f, float thickness = 2.f);

    // Template method for attack pattern (Template Method Pattern)
    void performAttack();
    void performSkill();

    // Animation support
    void setupPlayerAnimation(const std::string& spritesheetPath, int frameWidth, int frameHeight, int frameCount);
    
    // Template method for bullet management (defined in header for template instantiation)
    template<typename BulletType>
    void updateBulletContainer(std::vector<BulletType>& bullets, const Map* map) {
        for (size_t i = 0; i < bullets.size();) {
            bullets[i].update();
            sf::Vector2f bulletPos = bullets[i].getPosition();

            // Check wall collision first
            bool hitWall = false;
            if (map && !map->isPassable(bulletPos)) {
                hitWall = true;
            }

            // Remove bullet if it hits wall or goes out of bounds
            if (hitWall ||
                bulletPos.x < -100 || bulletPos.x > 3300 ||  // MAP_WIDTH * TILE_SIZE + buffer
                bulletPos.y < -100 || bulletPos.y > 2532) {  // MAP_HEIGHT * TILE_SIZE + buffer
                bullets.erase(bullets.begin() + i);
            } else {
                ++i;
            }
        }
    }

protected:
    // Hook methods for derived classes to implement
    virtual void executeAttack() = 0;
    virtual void executeSkill() = 0;



    // Movement functions
    void move(sf::Vector2f direction, const Map* map);
    bool checkCollision(sf::Vector2f newPosition, const Map* map) const;

public:
    // Getters (override BaseObject methods for backward compatibility)
    sf::Vector2f getPosition() const override { return shape.getPosition(); }
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    void setPosition(float x, float y) override {
        BaseObject::setPosition(x, y); // Keep sprite in sync first
        shape.setPosition(x, y);
    }
    int getLevel() const { return level; }
    int getExp() const { return exp; }
    int getExpToNextLevel() const { return expToNextLevel; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    bool getIsAlive() const override { return BaseObject::getIsAlive(); }
    float getSpeed() const { return speed; }
    std::string getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }

    // Combat getters/setters
    sf::Vector2f getAimDirection() const { return aimDirection; }
    void setAimDirection(sf::Vector2f direction) { aimDirection = direction; }
};

#endif
