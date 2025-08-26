#ifndef SWORD_H
#define SWORD_H

#include "Weapon.h"

class Sword : public Weapon {
private:
    sf::RectangleShape swordShape;
    sf::Vector2f swingDirection;
    sf::Clock swingTimer;
    bool isSwinging;
    float swingDuration;
    float swingAngle;
    float baseAngle;

    // Special skill properties
    bool isUsingSpecialSkill;
    sf::RectangleShape specialSlashShape;

public:
    Sword(float x, float y, sf::Vector2f direction);
    void update() override;
    void draw(sf::RenderWindow &window) override;
    
    // Sword-specific functions
    void startSwing(sf::Vector2f direction);
    bool isSwingActive() const { return isSwinging; }
    sf::FloatRect getSwingArea() const;
    void stopSwing();

    // Special skill functions
    void startSpecialSkill(sf::Vector2f direction);
    void stopSpecialSkill();
    sf::FloatRect getSpecialSkillArea() const;
    bool isSpecialSkillActive() const { return isUsingSpecialSkill; }
    
    // Override getBounds to return sword swing area when swinging
    sf::FloatRect getBounds() const;

    // Override setPosition to update both shapes
    void setPosition(sf::Vector2f position);
};

#endif
