#ifndef GUNHERO_H
#define GUNHERO_H

#include "Character.h"
#include "Bullet.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Map; // Forward declaration

class GunHero : public Character {
private:
    std::vector<Bullet> bullets;

public:
    GunHero();
    void update(const Map* map) override;
    void attack() override { performAttack(); }
    void skill() override { performSkill(); }

protected:
    // Hook methods implementation
    void executeAttack() override;
    void executeSkill() override;
    void draw(sf::RenderWindow &window) override;

public:
    // GunHero-specific functions
    std::vector<Bullet>& getBullets() { return bullets; }
    void updateBullets(const Map* map);
};

#endif
