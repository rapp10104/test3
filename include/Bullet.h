#ifndef BULLET_H
#define BULLET_H

#include "Weapon.h"

class Bullet : public Weapon {
public:
    Bullet(float x, float y, sf::Vector2f direction = sf::Vector2f(0.f, -1.f));
    void update() override;
    void draw(sf::RenderWindow &window) override;
};

#endif
