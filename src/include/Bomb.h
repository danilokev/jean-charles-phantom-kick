#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include "BoundingCircle.h"
#include "Bullet.h"
#include <vector>

class Bomb {
private:
    BoundingCircle area;
    float currentRadius;
    float maxRadius;
    float expansionSpeed;
    bool active;

public:
    Bomb();

    void launch(const sf::Vector2f& origin, float maxRadius = 300.f, float speed = 200.f);
    void update(float deltaTime);
    void render(sf::RenderTarget& target) const;
    void applyTo(std::vector<Bullet>& bullets);
    bool isActive() const;
};

#endif
