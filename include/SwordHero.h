#ifndef SWORDHERO_H
#define SWORDHERO_H

#include "Character.h"
#include "Sword.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Map; // Forward declaration

class SwordHero : public Character {
private:
    std::unique_ptr<Sword> sword;
    bool isUsingSpecialSkill;
    sf::Clock specialSkillTimer;
    bool hasUsedSkillBefore;

public:
    SwordHero();
    void update(const Map* map) override;
    void attack() override { performAttack(); } // chém thường
    void skill() override { performSkill(); }   // vòng chém AOE (unlock lv >= 10)

protected:
    // Hook methods implementation
    void executeAttack() override;
    void executeSkill() override;
    void draw(sf::RenderWindow &window) override;

public:
    // SwordHero-specific functions
    sf::FloatRect getAttackArea() const;
    bool canUseSkill() const override; // Override for special logic
    bool getIsAttacking() const;
    int getAttackDamage() const { return attackDamage; }
    Sword* getSword() { return sword.get(); }
};

#endif
