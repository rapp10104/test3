#ifndef MAGICBULLET_H
#define MAGICBULLET_H

#include "Weapon.h"

class MagicBullet : public Weapon {
public:
    MagicBullet(float x, float y, sf::Vector2f direction = sf::Vector2f(0.f, -1.f));
    void update() override;
    void draw(sf::RenderWindow &window) override;
};

#endif
