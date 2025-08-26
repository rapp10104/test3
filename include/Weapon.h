#ifndef WEAPON_H
#define WEAPON_H

#include "BaseObject.h"

enum class WeaponType {
    BULLET,
    MAGIC_BULLET,
    SWORD
};

class Weapon : public BaseObject {
protected:
    sf::CircleShape shape; // Keep for backward compatibility
    float speed;
    int damage;
    WeaponType weaponType;

public:
    Weapon(float x, float y, sf::Vector2f direction, WeaponType type);
    virtual ~Weapon() {}

    // Pure virtual functions that must be implemented by derived classes
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow &window) = 0;

    // Common functions (override BaseObject methods for backward compatibility)
    sf::Vector2f getPosition() const override;
    sf::FloatRect getBounds() const override;
    void setPosition(sf::Vector2f position);

    // Weapon-specific functions
    int getDamage() const { return damage; }
    WeaponType getType() const { return weaponType; }

protected:
    // Helper function for derived classes
    void calculateVelocity(sf::Vector2f direction);
    void moveWeapon();
};

#endif
