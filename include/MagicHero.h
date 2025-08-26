#ifndef MAGICHERO_H
#define MAGICHERO_H

#include "Character.h"
#include "MagicBullet.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Map; // Forward declaration

class MagicHero : public Character {
private:
    std::vector<MagicBullet> magicBullets;

public:
    MagicHero();
    void update(const Map* map) override;
    void attack() override { performAttack(); }
    void skill() override { performSkill(); }

protected:
    // Hook methods implementation
    void executeAttack() override;
    void executeSkill() override;
    void draw(sf::RenderWindow &window) override;

public:
    // MagicHero-specific functions
    std::vector<MagicBullet>& getBullets() { return magicBullets; }
    void updateBullets(const Map* map);
};

#endif
